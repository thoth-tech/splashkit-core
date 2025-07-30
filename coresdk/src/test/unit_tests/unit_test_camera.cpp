/**
 * Camera Tests
 */

#include "catch.hpp"

#include "camera.h"

using namespace splashkit_lib;
using Catch::Matchers::WithinRel;

TEST_CASE("Camera position correct after moving", "[camera][unit]")
{
    set_camera_position(point_at(42.0, 100.0));
    REQUIRE_THAT(camera_x(), WithinRel(42.0));
    REQUIRE_THAT(camera_y(), WithinRel(100.0));
}

TEST_CASE("Get screen center in world space", "[camera][graphics][window][integration]")
{
    open_window("Get screen center in world space", 100, 100);
    point_2d center = screen_center();
    REQUIRE_THAT(center.x, WithinRel(50.0));
    REQUIRE_THAT(center.y, WithinRel(50.0));
    close_current_window();
}

TEST_CASE("Convert world space to screen space", "[camera][unit]")
{
    set_camera_position(point_at(150.0, 150.0));
    
    SECTION("World x to screen space")
    {
        REQUIRE_THAT(to_screen_x(160.0), WithinRel(10.0));
    }
    SECTION("World y to screen space")
    {
        REQUIRE_THAT(to_screen_y(160.0), WithinRel(10.0));
    }
    SECTION("World point to screen space")
    {
        point_2d result = to_screen(point_at(160.0, 160.0));
        REQUIRE_THAT(result.x, WithinRel(10.0));
        REQUIRE_THAT(result.y, WithinRel(10.0));
    }
    SECTION("World vector to screen space")
    {
        vector_2d result = vector_world_to_screen();
        REQUIRE_THAT(result.x, WithinRel(-150.0));
        REQUIRE_THAT(result.y, WithinRel(-150.0));
    }
}

TEST_CASE("Convert screen space to world space", "[camera][unit]")
{
    set_camera_position(point_at(150.0, 150.0));

    SECTION("Screen x to world space")
    {
        REQUIRE_THAT(to_world_x(100.0), WithinRel(250.0));
    }
    SECTION("Screen y to world space")
    {
        REQUIRE_THAT(to_world_y(100.0), WithinRel(250.0));
    }
    SECTION("Screen point to world space")
    {
        point_2d result = to_world(point_at(100.0, 100.0));
        REQUIRE_THAT(result.x, WithinRel(250.0));
        REQUIRE_THAT(result.y, WithinRel(250.0));
    }
}

TEST_CASE("Check if rectangle is on screen", "[camera][rectangle_geometry][graphics][window][integration]")
{
    window wind = open_window("Check if rectangle is on screen", 100, 100);

    SECTION("50x50 rectangle at 0,0")
    {
        rectangle rect = rectangle_from(0.0, 0.0, 50.0, 50.0);

        SECTION("Rectangle is on screen")
        {
            REQUIRE(rect_on_screen(rect));
        }
        SECTION("Rectangle is in given window")
        {
            REQUIRE(rect_in_window(wind, rect));
        }
    }
    SECTION("50x50 rectangle at 100.1,100.1")
    {
        rectangle rect = rectangle_from(100.1, 100.1, 50.0, 50.0);

        SECTION("Rectangle is off screen")
        {
            REQUIRE_FALSE(rect_on_screen(rect));
        }
        SECTION("Rectangle is out of given window")
        {
            REQUIRE_FALSE(rect_in_window(wind, rect));
        }
    }
    close_window(wind);
}

TEST_CASE("Check if point is on screen", "[camera][point_geometry][graphics][window][integration]")
{
    window wind = open_window("Check if point is on screen", 100, 100);

    SECTION("Point at 50,50")
    {
        point_2d pt = point_at(50.0, 50.0);

        SECTION("Point is on screen")
        {
            REQUIRE(point_on_screen(pt));
        }
        SECTION("Point is in given window")
        {
            REQUIRE(point_in_window(wind, pt));
        }
    }
    SECTION("Point at 100.1,100.1")
    {
        point_2d pt = point_at(100.1, 100.1);

        SECTION("Point is off screen")
        {
            REQUIRE_FALSE(point_on_screen(pt));
        }
        SECTION("Point is out of given window")
        {
            REQUIRE_FALSE(point_in_window(wind, pt));
        }
    }
    close_window(wind);
}

TEST_CASE("Can move camera by offset", "[camera][unit]")
{
    move_camera_by(42.0, 100.0);
    REQUIRE_THAT(camera_x(), WithinRel(42.0));
    REQUIRE_THAT(camera_y(), WithinRel(100.0));
}

TEST_CASE("Can center camera on sprite", "[camera][bitmap][sprite][integration]")
{
    // Create 20x20 square sprite at 50,50 
    bitmap square_bmp = create_bitmap("square", 20, 20);
    sprite square_sprt = create_sprite(square_bmp);
    move_sprite_to(square_sprt, 50.0, 50.0);

    SECTION("Camera moves to sprite with no offset")
    {
        vector_2d offset = vector_to(0.0, 0.0);
        center_camera_on(square_sprt, offset);
        REQUIRE_THAT(camera_x(), WithinRel(50.0));
        REQUIRE_THAT(camera_y(), WithinRel(50.0));
    }
    SECTION("Camera moves to sprite with offset")
    {
        vector_2d offset = vector_to(0.5, 0.5);
        center_camera_on(square_sprt, offset);
        REQUIRE_THAT(camera_x(), WithinRel(50.5));
        REQUIRE_THAT(camera_y(), WithinRel(50.5));
    }
}