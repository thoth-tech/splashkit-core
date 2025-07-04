/**
 * Bitmap Unit Tests
 */

#include "catch.hpp"

#include "types.h"
#include "graphics.h"
#include "resources.h"

using namespace splashkit_lib;

constexpr double PI = 3.1415926535;

TEST_CASE("check that filled circle (using point_2d parameter) is producing the correct values for get pixel", "[fill_circle][get_pixel]")
{
    // The for loops and x/y calculations below loosely check the pixel color in a circle shape
    // Using a bitmap to avoid opening a window
    int radius = 100;
    point_2d circle_pt = point_at(200, 200);
    bitmap bmp = create_bitmap("fill_circle_at_point_bmp", 400, 400);
    color circle_clr = color_red();
    color background_clr = color_white();
    
    clear_bitmap(bmp, background_clr);
    for (int i = 0; i < 360; i++)
    {
        int x = circle_pt.x + (radius - 2) * cos(i * (PI / 180.0));
        int y = circle_pt.y + (radius - 2) * sin(i * (PI / 180.0));
        // Check background before
        REQUIRE(color_to_string(get_pixel(bmp, x, y)) == color_to_string(background_clr));
    }
    fill_circle(circle_clr, circle_pt, radius, option_draw_to(bmp));
    for (int i = 0; i < 360; i++)
    {
        int x = circle_pt.x + (radius - 2) * cos(i * (PI / 180.0));
        int y = circle_pt.y + (radius - 2) * sin(i * (PI / 180.0));
        // Check colour is now red from circle
        REQUIRE(color_to_string(get_pixel(bmp, x, y)) == color_to_string(circle_clr));
    }
    free_bitmap(bmp);
}
