/**
 * Camera Tests
 */

#include "catch.hpp"

#include "camera.h"

using namespace splashkit_lib;

TEST_CASE("Camera position correct after setting position", "[camera][integration]")
{
    set_camera_position(point_at(42.0, 100.0));

    point_2d result = camera_position();
    REQUIRE(result.x == Approx(42.0));
    REQUIRE(result.y == Approx(100.0));
}

TEST_CASE("Get screen center in world space", "[camera][graphics][window][integration]")
{
    open_window("", 100, 100);
    point_2d result = screen_center();
    REQUIRE(result.x == Approx(50.0));
    REQUIRE(result.y == Approx(50.0));
    close_current_window();
}

TEST_CASE("Convert world space to screen space", "[camera][unit]")
{
    set_camera_position(point_at(150.0, 150.0));
    
    SECTION("World x to screen space")
    {
        REQUIRE(to_screen_x(100.0) == Approx(50.0));
    }
    SECTION("World y to screen space")
    {
        REQUIRE(to_screen_y(100.0) == Approx(50.0));
    }
    SECTION("World point to screen space")
    {
        point_2d result = to_screen(point_at(100.0, 100.0));
        REQUIRE(result.x == Approx(50.0));
        REQUIRE(result.y == Approx(50.0));
    }
    SECTION("World vector to screen space")
    {
        vector_2d result = vector_world_to_screen();
        REQUIRE(result.x == Approx(-50.0));
        REQUIRE(result.y == Approx(-50.0));
    }
}

TEST_CASE("Convert rectangle to screen space", "[camera][rectangle_geometry][graphics][window][integration]")
{
    // TODO
}

TEST_CASE("Return window as rectangle", "[camera][rectangle_geometry][graphics][window][integration]")
{
    SECTION("Rectangle from screen")
    {
        // TODO

    }
    SECTION("Rectangle from window")
    {
        // TODO

    }
}

TEST_CASE("Convert screen space to world space", "[camera][unit]")
{
    SECTION("Screen x to world space")
    {
        // TODO
    }
    SECTION("Screen y to world space")
    {
        // TODO
    }
    SECTION("Screen point to world space")
    {
        // TODO
    }
}

TEST_CASE("Check if rectangle is on screen", "[camera][rectangle_geometry][graphics][window][integration]")
{
    SECTION("Rectangle is on screen")
    {
        SECTION("Rectangle is on screen")
        {
        // TODO

        }
        SECTION("Rectangle is in given window")
        {
        // TODO

        }
    }
    SECTION("Rectangle is off screen")
    {
        SECTION("Rectangle is off screen")
        {
            // TODO

        }
        SECTION("Rectangle is out of given window")
        {
            // TODO

        }
    }
}

TEST_CASE("Check if point is on screen", "[camera][point_geometry][graphics][window][integration]")
{
    SECTION("Point is on screen")
    {
        SECTION("Point is on screen")
        {
        // TODO

        }
        SECTION("Point is in given window")
        {
        // TODO

        }
    }
    SECTION("Point is off screen")
    {
        SECTION("Point is off screen")
        {
            // TODO

        }
        SECTION("Point is out of given window")
        {
            // TODO

        }
    }
}

TEST_CASE("Can move camera to coordinates", "[camera][unit]")
{
    // TODO
}

TEST_CASE("Can move camera by offset", "[camera][unit]")
{
    // TODO
}

TEST_CASE("Can center camera on sprite", "[camera][sprite][integration]")
{
    // TODO
}