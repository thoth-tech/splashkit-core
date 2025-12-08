/**
 * Graphics Unit Tests
 *
 * Tests for graphics/drawing functions using bitmap comparison helpers.
 */

#include "catch.hpp"

#include "graphics.h"
#include "window_manager.h"
#include "images.h"
#include "color.h"
#include "circle_drawing.h"
#include "rectangle_drawing.h"
#include "ellipse_drawing.h"
#include "line_drawing.h"
#include "drawing_options.h"

#include "graphics_test_helpers.h"

using namespace splashkit_lib;

// =============================================================================
// Helper Function Tests
// =============================================================================

TEST_CASE("color comparison works correctly", "[graphics][colors_match]")
{
    SECTION("identical colors match")
    {
        REQUIRE(colors_match(COLOR_RED, COLOR_RED));
        REQUIRE(colors_match(COLOR_WHITE, COLOR_WHITE));
        REQUIRE(colors_match(COLOR_BLACK, COLOR_BLACK));
    }
    SECTION("different colors do not match with zero tolerance")
    {
        REQUIRE_FALSE(colors_match(COLOR_RED, COLOR_BLUE, 0.0));
        REQUIRE_FALSE(colors_match(COLOR_WHITE, COLOR_BLACK, 0.0));
    }
    SECTION("similar colors match with tolerance")
    {
        // Colors that differ by small amounts should match with tolerance
        color almost_red = rgba_color(250, 0, 0, 255);  // Slightly less red
        REQUIRE(colors_match(COLOR_RED, almost_red, 0.05));
    }
    SECTION("color_difference returns expected values")
    {
        REQUIRE(color_difference(COLOR_RED, COLOR_RED) == 0.0);
        // Red vs Blue: R differs by 1.0, G same, B differs by 1.0, A same = average 0.5
        double diff = color_difference(COLOR_RED, COLOR_BLUE);
        REQUIRE(diff > 0.0);
        REQUIRE(diff <= 1.0);
    }
}

TEST_CASE("bitmap comparison works correctly", "[graphics][bitmaps_match]")
{
    bitmap bmp1 = create_bitmap("test_bmp1", 100, 100);
    bitmap bmp2 = create_bitmap("test_bmp2", 100, 100);

    SECTION("identical bitmaps match")
    {
        clear_bitmap(bmp1, COLOR_WHITE);
        clear_bitmap(bmp2, COLOR_WHITE);
        setup_collision_mask(bmp1);
        setup_collision_mask(bmp2);
        REQUIRE(bitmaps_match(bmp1, bmp2));
    }
    SECTION("different colored bitmaps do not match")
    {
        clear_bitmap(bmp1, COLOR_WHITE);
        clear_bitmap(bmp2, COLOR_RED);
        setup_collision_mask(bmp1);
        setup_collision_mask(bmp2);
        REQUIRE_FALSE(bitmaps_match(bmp1, bmp2));
    }
    SECTION("bitmap_diff_count returns correct count")
    {
        clear_bitmap(bmp1, COLOR_WHITE);
        clear_bitmap(bmp2, COLOR_WHITE);
        // Draw something different on bmp2
        fill_circle(COLOR_RED, 50, 50, 10, option_draw_to(bmp2));
        setup_collision_mask(bmp1);
        setup_collision_mask(bmp2);
        int diff = bitmap_diff_count(bmp1, bmp2);
        REQUIRE(diff > 0);
    }
    SECTION("bitmaps of different sizes do not match")
    {
        bitmap bmp3 = create_bitmap("test_bmp3", 50, 50);
        clear_bitmap(bmp1, COLOR_WHITE);
        clear_bitmap(bmp3, COLOR_WHITE);
        setup_collision_mask(bmp1);
        setup_collision_mask(bmp3);
        REQUIRE_FALSE(bitmaps_match(bmp1, bmp3));
        REQUIRE(bitmap_diff_count(bmp1, bmp3) == -1);
        free_bitmap(bmp3);
    }

    free_bitmap(bmp1);
    free_bitmap(bmp2);
}

// =============================================================================
// Shape Drawing Tests
// =============================================================================

TEST_CASE("fill_circle draws correctly", "[graphics][fill_circle]")
{
    bitmap bmp = create_bitmap("circle_test", 200, 200);
    color bg_color = COLOR_WHITE;
    color circle_color = COLOR_RED;
    point_2d center = point_at(100, 100);
    double radius = 40;

    clear_bitmap(bmp, bg_color);
    fill_circle(circle_color, center, radius, option_draw_to(bmp));
    setup_collision_mask(bmp);

    SECTION("circle is filled at center")
    {
        color pixel = get_pixel(bmp, 100, 100);
        REQUIRE(colors_match(pixel, circle_color, 0.1));
    }
    SECTION("circle is filled inside boundary")
    {
        // Check a point inside the circle (at half radius)
        int x = static_cast<int>(center.x + radius * 0.5);
        int y = static_cast<int>(center.y);
        color pixel = get_pixel(bmp, x, y);
        REQUIRE(colors_match(pixel, circle_color, 0.1));
    }
    SECTION("background is preserved outside circle")
    {
        // Check a point outside the circle 
        int x = static_cast<int>(center.x + radius * 1.5);
        int y = static_cast<int>(center.y);
        color pixel = get_pixel(bmp, x, y);
        REQUIRE(colors_match(pixel, bg_color, 0.1));
    }
    SECTION("verify_filled_circle helper works")
    {
        REQUIRE(verify_filled_circle(bmp, center, radius, circle_color, bg_color));
    }

    free_bitmap(bmp);
}

TEST_CASE("fill_circle with point_2d parameter works correctly", "[graphics][fill_circle]")
{
    bitmap bmp = create_bitmap("circle_pt_test", 200, 200);
    color bg_color = COLOR_WHITE;
    color circle_color = COLOR_BLUE;
    point_2d center = point_at(100, 100);
    double radius = 50;

    clear_bitmap(bmp, bg_color);

    SECTION("point is background before drawing")
    {
        setup_collision_mask(bmp);
        color pixel = get_pixel(bmp, 100, 100);
        REQUIRE(colors_match(pixel, bg_color, 0.1));
    }

    fill_circle(circle_color, center, radius, option_draw_to(bmp));
    setup_collision_mask(bmp);

    SECTION("point is filled after drawing")
    {
        color pixel = get_pixel(bmp, 100, 100);
        REQUIRE(colors_match(pixel, circle_color, 0.1));
    }
    SECTION("circle passes full verification")
    {
        REQUIRE(verify_filled_circle(bmp, center, radius, circle_color, bg_color));
    }

    free_bitmap(bmp);
}

TEST_CASE("fill_rectangle draws correctly", "[graphics][fill_rectangle]")
{
    bitmap bmp = create_bitmap("rect_test", 200, 200);
    color bg_color = COLOR_WHITE;
    color rect_color = COLOR_GREEN;
    rectangle rect = rectangle_from(50, 50, 100, 80);

    clear_bitmap(bmp, bg_color);
    fill_rectangle(rect_color, rect, option_draw_to(bmp));
    setup_collision_mask(bmp);

    SECTION("rectangle center is filled")
    {
        int cx = static_cast<int>(rect.x + rect.width / 2);
        int cy = static_cast<int>(rect.y + rect.height / 2);
        color pixel = get_pixel(bmp, cx, cy);
        REQUIRE(colors_match(pixel, rect_color, 0.1));
    }
    SECTION("verify_filled_rectangle helper works")
    {
        REQUIRE(verify_filled_rectangle(bmp, rect, rect_color));
    }
    SECTION("background is preserved outside rectangle")
    {
        color pixel = get_pixel(bmp, 10, 10);
        REQUIRE(colors_match(pixel, bg_color, 0.1));
    }

    free_bitmap(bmp);
}

TEST_CASE("fill_ellipse draws correctly", "[graphics][fill_ellipse]")
{
    bitmap bmp = create_bitmap("ellipse_test", 200, 200);
    color bg_color = COLOR_WHITE;
    color ellipse_color = COLOR_YELLOW;

    clear_bitmap(bmp, bg_color);
    // Draw ellipse at center (100,100) with width 80 and height 40
    fill_ellipse(ellipse_color, 60, 80, 80, 40, option_draw_to(bmp));
    setup_collision_mask(bmp);

    SECTION("ellipse center is filled")
    {
        color pixel = get_pixel(bmp, 100, 100);
        REQUIRE(colors_match(pixel, ellipse_color, 0.1));
    }
    SECTION("background is preserved outside ellipse")
    {
        color pixel = get_pixel(bmp, 10, 10);
        REQUIRE(colors_match(pixel, bg_color, 0.1));
    }

    free_bitmap(bmp);
}

TEST_CASE("draw_line draws correctly", "[graphics][draw_line]")
{
    bitmap bmp = create_bitmap("line_test", 200, 200);
    color bg_color = COLOR_WHITE;
    color line_color = COLOR_BLACK;

    clear_bitmap(bmp, bg_color);
    // Draw horizontal line from (50,100) to (150,100)
    draw_line(line_color, 50, 100, 150, 100, option_draw_to(bmp));
    setup_collision_mask(bmp);

    SECTION("line midpoint has correct color")
    {
        color pixel = get_pixel(bmp, 100, 100);
        REQUIRE(colors_match(pixel, line_color, 0.1));
    }
    SECTION("background is preserved away from line")
    {
        color pixel = get_pixel(bmp, 100, 50);
        REQUIRE(colors_match(pixel, bg_color, 0.1));
    }

    free_bitmap(bmp);
}

// =============================================================================
// Screen Dimension Tests
// =============================================================================

TEST_CASE("screen dimension utilities", "[screen_width][screen_height]")
{
    // Note: These tests require a window, so they are commented out
    // to allow running tests without a display
    // SECTION("check that screen width and screen height are correct")
    // {
    //     window w = open_window("test window", 200, 100);
    //     REQUIRE(screen_width() == 200);
    //     REQUIRE(screen_height() == 100);
    //     close_window(w);
    // }
}
