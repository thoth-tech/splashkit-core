/**
 * Bitmap Unit Tests
 */

#include "catch.hpp"

#include "types.h"
#include "graphics.h"
#include "resources.h"

#include "logging_handling.h"

using namespace splashkit_lib;

constexpr int ROCKET_WIDTH = 36, ROCKET_HEIGHT = 72,
              BACKGROUND_WIDTH = 864, BACKGROUND_HEIGHT = 769,
              FROG_WIDTH = 294, FROG_HEIGHT = 422;

TEST_CASE("bitmaps can be created and freed", "[load_bitmap][bitmap_width][bitmap_height][free_bitmap]")
{
    // Creating bitmaps
    bitmap rocket_bmp, frog_bmp, background_bmp;
    rocket_bmp = load_bitmap("rocket_sprt", "rocket_sprt.png");
    REQUIRE(bitmap_valid(rocket_bmp));
    REQUIRE(rocket_bmp != nullptr);
    REQUIRE(bitmap_width(rocket_bmp) == ROCKET_WIDTH);
    REQUIRE(bitmap_height(rocket_bmp) == ROCKET_HEIGHT);
    frog_bmp = load_bitmap("frog", "frog.png");
    REQUIRE(bitmap_valid(frog_bmp));
    REQUIRE(frog_bmp != nullptr);
    REQUIRE(bitmap_width(frog_bmp) == FROG_WIDTH);
    REQUIRE(bitmap_height(frog_bmp) == FROG_HEIGHT);
    background_bmp = load_bitmap("background", "background.png");
    REQUIRE(bitmap_valid(background_bmp));
    REQUIRE(background_bmp != nullptr);
    REQUIRE(bitmap_width(background_bmp) == BACKGROUND_WIDTH);
    REQUIRE(bitmap_height(background_bmp) == BACKGROUND_HEIGHT);

    // Freeing bitmaps
    free_bitmap(rocket_bmp);
    REQUIRE_FALSE(has_bitmap("rocket_sprt"));
    free_bitmap(frog_bmp);
    REQUIRE_FALSE(has_bitmap("frog"));
    free_bitmap(background_bmp);
    REQUIRE_FALSE(has_bitmap("background"));
}

TEST_CASE("can detect non-existent bitmap")
{
    REQUIRE(has_bitmap("non_existent") == false);

    disable_logging(WARNING); // Disables "WARNING -> Unable to locate file for non_existent"
    bitmap no_bmp = load_bitmap("non_existent", "non_existent.jpg");
    enable_logging(WARNING);
    
    REQUIRE(no_bmp == nullptr);
    REQUIRE(has_bitmap("non_existent") == false);
}

TEST_CASE("can load and free multiple bitmaps")
{
    bitmap rocket_bmp, frog_bmp, background_bmp;
    rocket_bmp = load_bitmap("rocket_sprt", "rocket_sprt.png");
    frog_bmp = load_bitmap("frog", "frog.png");
    background_bmp = load_bitmap("background", "background.png");
    REQUIRE(bitmap_valid(rocket_bmp));
    REQUIRE(bitmap_valid(frog_bmp));
    REQUIRE(bitmap_valid(background_bmp));

    // Freeing all bitmaps
    free_all_bitmaps();

    REQUIRE_FALSE(bitmap_valid(rocket_bmp));
    REQUIRE_FALSE(bitmap_valid(frog_bmp));
    REQUIRE_FALSE(bitmap_valid(background_bmp));
}

TEST_CASE("bitmap bounding details can be retrieved", "[bitmap]")
{
    bitmap bmp = load_bitmap("rocket", "rocket_sprt.png");
    REQUIRE(bmp != nullptr);
    REQUIRE(bitmap_valid(bmp));
    SECTION("can get bitmap width")
    {
        REQUIRE(bitmap_width(bmp) == ROCKET_WIDTH);
    }
    SECTION("can get bitmap height")
    {
        REQUIRE(bitmap_height(bmp) == ROCKET_HEIGHT);
    }
    SECTION("can get bitmap center")
    {
        point_2d center = bitmap_center(bmp);
        REQUIRE(center.x == ROCKET_WIDTH / 2.0);
        REQUIRE(center.y == ROCKET_HEIGHT / 2.0);
    }
    SECTION("can get bitmap bounding rectangle")
    {
        rectangle rect = bitmap_bounding_rectangle(bmp);
        REQUIRE(rect.x == 0.0);
        REQUIRE(rect.y == 0.0);
        REQUIRE(rect.width == ROCKET_WIDTH);
        REQUIRE(rect.height == ROCKET_HEIGHT);
    }
    double center_corner_dist = sqrt(pow(ROCKET_WIDTH / 2.0, 2.0) + pow(ROCKET_HEIGHT / 2.0, 2.0));

    SECTION("can get bitmap bounding circle")
    {
        circle circ = bitmap_bounding_circle(bmp, point_at(100.0, 100.0));
        REQUIRE(circ.center.x == 100.0);
        REQUIRE(circ.center.y == 100.0);
        REQUIRE(circ.radius == center_corner_dist);
    }
    SECTION("can get bitmap cell circle")
    {
        point_2d pt = point_at(100.0, 100.0);
        circle circ = bitmap_cell_circle(bmp, pt);
        REQUIRE(circ.center.x == pt.x);
        REQUIRE(circ.center.y == pt.y);
        REQUIRE(circ.radius == center_corner_dist);
        circle circ2 = bitmap_cell_circle(bmp, pt.x, pt.y);
        REQUIRE(circ2.center.x == pt.x);
        REQUIRE(circ2.center.y == pt.y);
        REQUIRE(circ2.radius == center_corner_dist);

        SECTION("can get bitmap cell circle with scale")
        {
            double scale = 2.0;
            circle circ2 = bitmap_cell_circle(bmp, pt, scale);
            REQUIRE(circ2.center.x == pt.x);
            REQUIRE(circ2.center.y == pt.y);
            REQUIRE(circ2.radius == center_corner_dist * scale);
        }
    }
    free_bitmap(bmp);
}

TEST_CASE("bitmaps can detect point collisions", "[bitmap][collision]")
{
    // Create a 2x2 bitmap for testing
    // P1 P2
    // P3 P4
    // Where P1 is Opaque, P2 is Transparent (alpha < 0x7F), P3 is Opaque, P4 is Transparent
    bitmap test_bmp = create_bitmap("collision_test", 2, 2);
    clear_bitmap(test_bmp, COLOR_TRANSPARENT);

    // Set pixels
    draw_pixel_on_bitmap(test_bmp, COLOR_RED, 0, 0); // P1 Opaque
    draw_pixel_on_bitmap(test_bmp, rgba_color(255, 0, 0, 100), 1, 0); // P2 Transparent
    draw_pixel_on_bitmap(test_bmp, COLOR_RED, 0, 1); // P3 Opaque
    draw_pixel_on_bitmap(test_bmp, rgba_color(255, 0, 0, 50), 1, 1); // P4 Transparent

    setup_collision_mask(test_bmp);

    SECTION("point collision with bitmap at origin")
    {
        // P1
        REQUIRE(bitmap_point_collision(test_bmp, 0, 0, 0.5, 0.5));
        // P2
        REQUIRE_FALSE(bitmap_point_collision(test_bmp, 0, 0, 1.5, 0.5));
        // P3
        REQUIRE(bitmap_point_collision(test_bmp, 0, 0, 0.5, 1.5));
        // P4
        REQUIRE_FALSE(bitmap_point_collision(test_bmp, 0, 0, 1.5, 1.5));
    }

    SECTION("point collision with bitmap at offset")
    {
        double offset_x = 100.0;
        double offset_y = 50.0;
        // P1
        REQUIRE(bitmap_point_collision(test_bmp, offset_x, offset_y, offset_x + 0.5, offset_y + 0.5));
        // P2
        REQUIRE_FALSE(bitmap_point_collision(test_bmp, offset_x, offset_y, offset_x + 1.5, offset_y + 0.5));
        // Outside bitmap
        REQUIRE_FALSE(bitmap_point_collision(test_bmp, offset_x, offset_y, offset_x - 1, offset_y));
        REQUIRE_FALSE(bitmap_point_collision(test_bmp, offset_x, offset_y, offset_x + 3, offset_y + 1));
    }

    free_bitmap(test_bmp);
}

TEST_CASE("bitmaps can detect rectangle and circle collisions", "[bitmap][collision]")
{
    // Create a 4x4 bitmap for testing with an opaque 2x2 square at the center
    bitmap test_bmp = create_bitmap("rect_collision_test", 4, 4);
    clear_bitmap(test_bmp, COLOR_TRANSPARENT);

    // Draw opaque 2x2 square from (1,1) to (2,2)
    draw_pixel_on_bitmap(test_bmp, COLOR_RED, 1, 1);
    draw_pixel_on_bitmap(test_bmp, COLOR_RED, 2, 1);
    draw_pixel_on_bitmap(test_bmp, COLOR_RED, 1, 2);
    draw_pixel_on_bitmap(test_bmp, COLOR_RED, 2, 2);

    setup_collision_mask(test_bmp);

    SECTION("rectangle collisions")
    {
        // Transparent region overlap only
        REQUIRE_FALSE(bitmap_rectangle_collision(test_bmp, 0, 0, rectangle_from(0, 0, 1, 1)));
        // Opaque region overlap
        REQUIRE(bitmap_rectangle_collision(test_bmp, 0, 0, rectangle_from(0.5, 0.5, 1, 1)));
        // Entirely outside
        REQUIRE_FALSE(bitmap_rectangle_collision(test_bmp, 0, 0, rectangle_from(5, 5, 2, 2)));
        // Rectangle contains the whole bitmap
        REQUIRE(bitmap_rectangle_collision(test_bmp, 0, 0, rectangle_from(-1, -1, 6, 6)));
    }

    SECTION("circle collisions")
    {
        // Transparent region overlap only
        REQUIRE_FALSE(bitmap_circle_collision(test_bmp, 0, 0, circle_at(0.5, 0.5, 0.4)));
        // Opaque region overlap
        REQUIRE(bitmap_circle_collision(test_bmp, 0, 0, circle_at(1.5, 1.5, 0.1)));
        // Touching the edge of opaque pixel
        REQUIRE(bitmap_circle_collision(test_bmp, 0, 0, circle_at(0.5, 1.5, 0.6)));
        // Entirely outside
        REQUIRE_FALSE(bitmap_circle_collision(test_bmp, 0, 0, circle_at(10, 10, 1)));
    }

    free_bitmap(test_bmp);
}

TEST_CASE("bitmaps can detect bitmap-to-bitmap collisions", "[bitmap][collision]")
{
    // Create two 2x2 bitmaps
    // B1: 2x2 opaque
    bitmap bmp1 = create_bitmap("bmp1", 2, 2);
    clear_bitmap(bmp1, COLOR_RED);
    setup_collision_mask(bmp1);

    // B2: 2x2 with only one opaque pixel at (0,0)
    bitmap bmp2 = create_bitmap("bmp2", 2, 2);
    clear_bitmap(bmp2, COLOR_TRANSPARENT);
    draw_pixel_on_bitmap(bmp2, COLOR_RED, 0, 0);
    setup_collision_mask(bmp2);

    SECTION("collision when pixels overlap")
    {
        // Overlap (0,0) of bmp2 with (1,1) of bmp1
        REQUIRE(bitmap_collision(bmp1, 0, 0, bmp2, 1, 1));
    }

    SECTION("no collision when bounding boxes overlap but pixels don't")
    {
        // B3: 2x2 with only one opaque pixel at (1,1)
        bitmap bmp3 = create_bitmap("bmp3", 2, 2);
        clear_bitmap(bmp3, COLOR_TRANSPARENT);
        draw_pixel_on_bitmap(bmp3, COLOR_RED, 1, 1);
        setup_collision_mask(bmp3);

        // Position bmp2 and bmp3 such that their bounding boxes overlap at (1,1) relative to world
        // bmp2 at (1,1) -> its opaque pixel (0,0) is at world (1,1)
        // bmp3 at (0,0) -> its opaque pixel (1,1) is at world (1,1)
        // They should collide.
        REQUIRE(bitmap_collision(bmp2, 1, 1, bmp3, 0, 0));

        // Position bmp2 and bmp3 such that they overlap but transparent pixels meet
        // bmp2 at (0,0) -> opaque (0,0), transparent (1,1)
        // bmp3 at (0,0) -> transparent (0,0), opaque (1,1)
        // Bounding boxes are identical but pixels (0,0) and (1,1) don't overlap.
        REQUIRE_FALSE(bitmap_collision(bmp2, 0, 0, bmp3, 0, 0));

        free_bitmap(bmp3);
    }

    SECTION("no collision when entirely separate")
    {
        REQUIRE_FALSE(bitmap_collision(bmp1, 0, 0, bmp2, 10, 10));
    }

    free_bitmap(bmp1);
    free_bitmap(bmp2);
}
