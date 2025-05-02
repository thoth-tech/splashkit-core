/**
 * unit_test_graphics.cpp
 * Unit tests for SplashKit graphics utilities.
 */

#include "catch.hpp"
#include "graphics.h"
#include <string>

using namespace splashkit_lib;
using namespace std;

TEST_CASE("screen dimension utilities", "[graphics][screen]")
{
    SECTION("screen width and height should be positive")
    {
        REQUIRE(screen_width() > 0);
        REQUIRE(screen_height() > 0);
    }
}

TEST_CASE("color creation and properties", "[graphics][color]")
{
    SECTION("can create color with RGBA values")
    {
        color c = create_color(123, 45, 67, 200);
        REQUIRE(c.r == 123);
        REQUIRE(c.g == 45);
        REQUIRE(c.b == 67);
        REQUIRE(c.a == 200);
    }

    SECTION("create_color defaults to alpha 255 when omitted")
    {
        color c = create_color(10, 20, 30);
        REQUIRE(c.r == 10);
        REQUIRE(c.g == 20);
        REQUIRE(c.b == 30);
        REQUIRE(c.a == 255);
    }
}

TEST_CASE("string-to-color conversion", "[graphics][color]")
{
    SECTION("known color strings return expected colors")
    {
        color red = string_to_color("RED");
        REQUIRE(red.r == 255);
        REQUIRE(red.g == 0);
        REQUIRE(red.b == 0);
        REQUIRE(red.a == 255);

        color blue = string_to_color("BLUE");
        REQUIRE(blue.r == 0);
        REQUIRE(blue.g == 0);
        REQUIRE(blue.b == 255);
        REQUIRE(blue.a == 255);
    }

    SECTION("invalid color string returns black")
    {
        color invalid = string_to_color("not_a_color");
        REQUIRE(invalid.r == 0);
        REQUIRE(invalid.g == 0);
        REQUIRE(invalid.b == 0);
        REQUIRE(invalid.a == 255);
    }
}

TEST_CASE("color-to-string conversion", "[graphics][color]")
{
    SECTION("known colors convert to their string names")
    {
        REQUIRE(color_to_string(COLOR_GREEN) == "GREEN");
        REQUIRE(color_to_string(COLOR_PURPLE) == "PURPLE");
    }

    SECTION("unknown/custom color returns hex string")
    {
        color custom = create_color(12, 34, 56, 78);
        string result = color_to_string(custom);

        // Result should start with '#' and have 8 hex digits
        REQUIRE(result.length() == 9); // "#RRGGBBAA"
        REQUIRE(result[0] == '#');
    }
}
