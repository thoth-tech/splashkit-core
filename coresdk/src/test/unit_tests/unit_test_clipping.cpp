/**
 * Clipping Unit Tests
 * 
 * 
 * Created by Matthew Sutrisno
 */

#include "graphics.h"
#include "color.h"
#include "catch.hpp"

using namespace splashkit_lib;


static bool pixel_is(bitmap bmp, int x, int y, color expected)
{
    color c = get_pixel(bmp, x, y);
    
    return (red_of(c) == red_of(expected) &&
            green_of(c) == green_of(expected) &&
            blue_of(c) == blue_of(expected) &&
            alpha_of(c) == alpha_of(expected));
}

TEST_CASE("set_clip restricts drawing to area", "[graphics][clipping]")
{
    bitmap bmp = create_bitmap("clip_test1", 40, 40);
    clear_bitmap(bmp, COLOR_WHITE);

    set_clip(bmp, rectangle_from(10, 10, 20, 20));

    fill_rectangle(COLOR_RED, 0, 0, 40, 40, option_draw_to(bmp));

    REQUIRE(pixel_is(bmp, 15, 15, COLOR_RED));

    REQUIRE(pixel_is(bmp, 5, 5, COLOR_WHITE));
    REQUIRE(pixel_is(bmp, 35, 35, COLOR_WHITE));

    free_bitmap(bmp);
}

TEST_CASE("push_clip intersects clipping regions", "[graphics][clipping]")
{
    bitmap bmp = create_bitmap("clip_test2", 40, 40);
    clear_bitmap(bmp, COLOR_WHITE);

    set_clip(bmp, rectangle_from(10, 10, 20, 20));

    push_clip(bmp, rectangle_from(20, 0, 10, 40));

    fill_rectangle(COLOR_BLUE, 0, 0, 40, 40, option_draw_to(bmp));

    REQUIRE(pixel_is(bmp, 25, 15, COLOR_BLUE));

    REQUIRE(pixel_is(bmp, 15, 15, COLOR_WHITE));

    REQUIRE(pixel_is(bmp, 5, 5, COLOR_WHITE));

    free_bitmap(bmp);
}

TEST_CASE("pop_clip restores previous clipping region", "[graphics][clipping]")
{
    bitmap bmp = create_bitmap("clip_test3", 40, 40);
    clear_bitmap(bmp, COLOR_WHITE);

    set_clip(bmp, rectangle_from(5, 5, 20, 20));

    push_clip(bmp, rectangle_from(10, 10, 20, 20));

    fill_rectangle(COLOR_GREEN, 0, 0, 40, 40, option_draw_to(bmp));

    REQUIRE(pixel_is(bmp, 12, 12, COLOR_GREEN));
    REQUIRE(pixel_is(bmp, 6, 6, COLOR_WHITE));

    pop_clip(bmp);

    fill_rectangle(COLOR_RED, 0, 0, 40, 40, option_draw_to(bmp));

    REQUIRE(pixel_is(bmp, 6, 6, COLOR_RED));

    REQUIRE(pixel_is(bmp, 30, 30, COLOR_WHITE));

    free_bitmap(bmp);
}

TEST_CASE("reset_clip removes clipping", "[graphics][clipping]")
{
    bitmap bmp = create_bitmap("clip_test4", 40, 40);
    clear_bitmap(bmp, COLOR_WHITE);

    set_clip(bmp, rectangle_from(10, 10, 10, 10));

    reset_clip(bmp);

    fill_rectangle(COLOR_GOLD, 0, 0, 40, 40, option_draw_to(bmp));

    REQUIRE(pixel_is(bmp, 0, 0, COLOR_GOLD));
    REQUIRE(pixel_is(bmp, 39, 39, COLOR_GOLD));

    free_bitmap(bmp);
}
