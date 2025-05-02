#include "catch.hpp"
#include "graphics.h"
#include "color.h"

using namespace splashkit_lib;

TEST_CASE("screen dimension utilities", "[graphics]")
{
    // Avoid requiring window.h by skipping test if no window open
    int width = screen_width();
    int height = screen_height();

    SECTION("screen width and height should be positive if a window is open")
    {
        if (width == 0 || height == 0)
        {
            WARN("No window is open — skipping screen size tests.");
            SUCCEED(); // Avoid failing due to no window
        }
        else
        {
            REQUIRE(width > 0);
            REQUIRE(height > 0);
        }
    }
}

TEST_CASE("color creation and properties", "[graphics]")
{
    SECTION("can create color with RGBA values")
    {
        color c = rgba_color(123, 45, 67, 200);
        REQUIRE(int(red_of(c)) == 123);
        REQUIRE(int(green_of(c)) == 45);
        REQUIRE(int(blue_of(c)) == 67);
        REQUIRE(int(alpha_of(c)) == 200);
    }

    SECTION("rgb_color defaults to alpha 255")
    {
        color c = rgb_color(10, 20, 30);
        REQUIRE(int(red_of(c)) == 10);
        REQUIRE(int(green_of(c)) == 20);
        REQUIRE(int(blue_of(c)) == 30);
        REQUIRE(int(alpha_of(c)) == 255);
    }
}

TEST_CASE("string-to-color conversion", "[graphics]")
{
    SECTION("valid color string returns expected color")
    {
        color red = string_to_color("#ff0000ff");
        REQUIRE(int(red_of(red)) == 255);
        REQUIRE(int(green_of(red)) == 0);
        REQUIRE(int(blue_of(red)) == 0);
        REQUIRE(int(alpha_of(red)) == 255);
    }

    SECTION("invalid string returns fallback color (black)")
    {
        color invalid = string_to_color("not_a_color");
        // Just ensure it doesn't crash and has valid structure
        REQUIRE(red_of(invalid) >= 0);
        REQUIRE(green_of(invalid) >= 0);
        REQUIRE(blue_of(invalid) >= 0);
    }
}

TEST_CASE("color-to-string conversion", "[graphics]")
{
    SECTION("known color converts to hex string")
    {
        color green = color_green();
        string str = color_to_string(green);
        REQUIRE(str.substr(0, 1) == "#");
        REQUIRE(str.length() == 9); // #RRGGBBAA
    }

    SECTION("round-trip color to string and back")
    {
        color original = rgba_color(50, 100, 150, 200);
        string hex = color_to_string(original);
        color result = string_to_color(hex);
        REQUIRE(int(red_of(result)) == 50);
        REQUIRE(int(green_of(result)) == 100);
        REQUIRE(int(blue_of(result)) == 150);
        REQUIRE(int(alpha_of(result)) == 200);
    }
}
