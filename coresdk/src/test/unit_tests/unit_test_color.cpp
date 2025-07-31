/**
 * Color Unit Tests
 */

#include "catch.hpp"

#include "color.h"

using namespace splashkit_lib;

TEST_CASE("can create color with RGB(A) values", "[rgba_color][rgb_color][red_of][blue_of][green_of][alpha_of]")
{
    SECTION("can create RGB color with variable alpha values")
    {
        color c = rgba_color(123, 45, 67, 200);
        REQUIRE(red_of(c) == 123);
        REQUIRE(green_of(c) == 45);
        REQUIRE(blue_of(c) == 67);
        REQUIRE(alpha_of(c) == 200);
    }

    SECTION("can create RGB color with alpha of 255 (opaque)")
    {
        color c = rgb_color(10, 20, 30);
        REQUIRE(red_of(c) == 10);
        REQUIRE(green_of(c) == 20);
        REQUIRE(blue_of(c) == 30);
        REQUIRE(alpha_of(c) == 255);
    }
}

TEST_CASE("can convert a string to a color", "[string_to_color][red_of][blue_of][green_of][alpha_of]")
{
    SECTION("valid color hex string returns expected color values")
    {
        color red = string_to_color("#ff0000ff");
        REQUIRE(red_of(red) == 255);
        REQUIRE(green_of(red) == 0);
        REQUIRE(blue_of(red) == 0);
        REQUIRE(alpha_of(red) == 255);
    }

    SECTION("invalid string returns fallback color of white")
    {
        color invalid = string_to_color("not_a_color");
        REQUIRE(red_of(invalid) == 255);
        REQUIRE(green_of(invalid) == 255);
        REQUIRE(blue_of(invalid) == 255);
    }
}

TEST_CASE("can convert a color to a hex string", "[color_to_string][color_green]")
{
    SECTION("color green converts to hex string correctly")
    {
        color green = color_green();
        string str = color_to_string(green);
        REQUIRE(str == "#007f00ff");
    }

    SECTION("round-trip color to hex string and back")
    {
        color original = rgba_color(50, 100, 150, 200);
        string hex = color_to_string(original);
        color result = string_to_color(hex);
        REQUIRE(red_of(result) == 50);
        REQUIRE(green_of(result) == 100);
        REQUIRE(blue_of(result) == 150);
        REQUIRE(alpha_of(result) == 200);
    }
}

TEST_CASE("hsb_color converts HSB to RGB correctly", "[hsb_color]")
{
    SECTION("black when brightness is 0")
    {
        color black = hsb_color(0.5, 1.0, 0.0);
        REQUIRE(red_of(black) == 0);
        REQUIRE(green_of(black) == 0);
        REQUIRE(blue_of(black) == 0);
    }

    SECTION("gray when saturation is 0")
    {
        color gray = hsb_color(0.3, 0.0, 0.5);
        REQUIRE(red_of(gray) == 127);
        REQUIRE(green_of(gray) == 127);
        REQUIRE(blue_of(gray) == 127);
    }

    SECTION("pure red from HSB")
    {   
        color red = hsb_color(0.0, 1.0, 1.0);
        REQUIRE(red_of(red) == 255);
        REQUIRE(green_of(red) == 0);
        REQUIRE(blue_of(red) == 0);
    }

    SECTION("pure green from HSB")
    {
        color green = hsb_color(1.0 / 3.0, 1.0, 1.0);
        REQUIRE(red_of(green) == 0);
        REQUIRE(green_of(green) == 255);
        REQUIRE(blue_of(green) == 0);
    }

    SECTION("pure blue from HSB")
    {
        color blue = hsb_color(2.0 / 3.0, 1.0, 1.0);
        REQUIRE(red_of(blue) == 0);
        REQUIRE(green_of(blue) == 0);
        REQUIRE(blue_of(blue) == 255);
    }

    SECTION("pure yellow from HSB")
    {
        color yellow = hsb_color(1.0 / 6.0, 1.0, 1.0);
        REQUIRE(red_of(yellow) == 255);
        REQUIRE(green_of(yellow) == 255);
        REQUIRE(blue_of(yellow) == 0);
    }

    SECTION("pure cyan from HSB")
    {
        color cyan = hsb_color(0.5, 1.0, 1.0);
        REQUIRE(red_of(cyan) == 0);
        REQUIRE(green_of(cyan) == 255);
        REQUIRE(blue_of(cyan) == 255);
    }

    SECTION("pure magenta from HSB")
    {
        color magenta = hsb_color(5.0 / 6.0, 1.0, 1.0);
        REQUIRE(red_of(magenta) == 255);
        REQUIRE(green_of(magenta) == 0);
        REQUIRE(blue_of(magenta) == 255);
    }
}
