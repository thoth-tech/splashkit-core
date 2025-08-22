/**
 * Camera Tests
 */

#include "catch.hpp"

#include "camera.h"
#include <limits.h>

using namespace splashkit_lib;
using Catch::Matchers::WithinRel;

// Fixture to reset camera position for tests
class CameraTestFixture {
    public:
        CameraTestFixture()
        {
            set_camera_position(point_at(0.0, 0.0));
        }
};


TEST_CASE_METHOD(CameraTestFixture, "camera position correct after moving", "[set_camera_position]")
{
    SECTION("to positive coordinates")
    {
        set_camera_position(point_at(42.0, 100.0));
        REQUIRE_THAT(camera_x(), WithinRel(42.0));
        REQUIRE_THAT(camera_y(), WithinRel(100.0));
    }
    SECTION("to negative coordinates")
    {
        set_camera_position(point_at(-42.0, -100.0));
        REQUIRE_THAT(camera_x(), WithinRel(-42.0));
        REQUIRE_THAT(camera_y(), WithinRel(-100.0));
    }
    SECTION("consecutive movements")
    {
        set_camera_position(point_at(-42.0, -100.0));
        set_camera_position(point_at(100.0, 300.0));
        REQUIRE_THAT(camera_x(), WithinRel(100.0));
        REQUIRE_THAT(camera_y(), WithinRel(300.0));
    }
}

TEST_CASE_METHOD(CameraTestFixture, "get screen center in world space", "[screen_center]")
{
    open_window("get screen center in world space", 100, 100);
    point_2d center = screen_center();
    REQUIRE_THAT(center.x, WithinRel(50.0));
    REQUIRE_THAT(center.y, WithinRel(50.0));
    close_current_window();
}

TEST_CASE_METHOD(CameraTestFixture, "convert world space to screen space", "[to_screen_x][to_screen_y][to_screen][vector_world_to_screen]")
{
    set_camera_position(point_at(150.0, 150.0));
    
    SECTION("world x to screen space")
    {
        REQUIRE_THAT(to_screen_x(160.0), WithinRel(10.0));
    }
    SECTION("world y to screen space")
    {
        REQUIRE_THAT(to_screen_y(160.0), WithinRel(10.0));
    }
    SECTION("world point to screen space")
    {
        point_2d result = to_screen(point_at(160.0, 160.0));
        REQUIRE_THAT(result.x, WithinRel(10.0));
        REQUIRE_THAT(result.y, WithinRel(10.0));
    }
    SECTION("world vector to screen space")
    {
        vector_2d result = vector_world_to_screen();
        REQUIRE_THAT(result.x, WithinRel(-150.0));
        REQUIRE_THAT(result.y, WithinRel(-150.0));
    }
}

TEST_CASE_METHOD(CameraTestFixture, "convert screen space to world space", "[to_world_x][to_world_y][to_world]")
{
    set_camera_position(point_at(150.0, 150.0));

    SECTION("screen x to world space")
    {
        REQUIRE_THAT(to_world_x(100.0), WithinRel(250.0));
    }
    SECTION("screen y to world space")
    {
        REQUIRE_THAT(to_world_y(100.0), WithinRel(250.0));
    }
    SECTION("screen point to world space")
    {
        point_2d result = to_world(point_at(100.0, 100.0));
        REQUIRE_THAT(result.x, WithinRel(250.0));
        REQUIRE_THAT(result.y, WithinRel(250.0));
    }
}

TEST_CASE_METHOD(CameraTestFixture, "check if rectangle is on screen", "[rect_on_screen]")
{
    window wind = open_window("check if rectangle is on screen", 100, 100);

    SECTION("rectangle on screen")
    {
        rectangle rect = rectangle_from(0.0, 0.0, 50.0, 50.0);
        REQUIRE(rect_on_screen(rect));
    }
    SECTION("rectangle partially on screen")
    {
        rectangle rect = rectangle_from(75.0, 75.0, 50.0, 50.0);
        REQUIRE(rect_on_screen(rect));
    }
    SECTION("rectangle touches border of screen, counts as off screen")
    {
        rectangle rect = rectangle_from(100.0, 100.0, 50.0, 50.0);
        REQUIRE_FALSE(rect_on_screen(rect));
    }
    SECTION("rectangle off screen")
    {
        rectangle rect = rectangle_from(100.1, 100.1, 50.0, 50.0);
        REQUIRE_FALSE(rect_on_screen(rect));
    }
}

TEST_CASE_METHOD(CameraTestFixture, "check if rectangle is in window", "[rect_in_window]")
{
    window wind = open_window("check if rectangle is in window", 100, 100);

    SECTION("rectangle in window")
    {
        rectangle rect = rectangle_from(0.0, 0.0, 50.0, 50.0);
        REQUIRE(rect_in_window(wind, rect));
    }
    SECTION("rectangle partially in window")
    {
        rectangle rect = rectangle_from(75.0, 75.0, 50.0, 50.0);
        REQUIRE(rect_in_window(wind, rect));
    }
    SECTION("rectangle touches border of screen, counts as out of window")
    {
        rectangle rect = rectangle_from(100.0, 100.0, 50.0, 50.0);
        REQUIRE_FALSE(rect_in_window(wind, rect));
    }
    SECTION("rectangle out of window")
    {
        rectangle rect = rectangle_from(100.1, 100.1, 50.0, 50.0);
        REQUIRE_FALSE(rect_in_window(wind, rect));
    }
    close_window(wind);
}

TEST_CASE_METHOD(CameraTestFixture, "check if point is on screen", "[point_on_screen]")
{
    window wind = open_window("check if point is on screen", 100, 100);

    SECTION("point is on screen")
    {
        point_2d pt = point_at(50.0, 50.0);
        REQUIRE(point_on_screen(pt));
    }
    SECTION("point is on edge of screen, counts as on screen")
    {
        point_2d pt = point_at(100.0, 100.0);
        REQUIRE(point_on_screen(pt));
    }
    SECTION("point is off screen")
    {
        point_2d pt = point_at(100.1, 100.1);
        REQUIRE_FALSE(point_on_screen(pt));
    }
    close_window(wind);
}

TEST_CASE_METHOD(CameraTestFixture, "check if point is in window", "[point_in_window]")
{
    window wind = open_window("check if point is in window", 100, 100);

    SECTION("point is in window")
    {
        point_2d pt = point_at(50.0, 50.0);
        REQUIRE(point_in_window(wind, pt));
    }
    SECTION("point is on edge of screen, counts as in window")
    {
        point_2d pt = point_at(100.0, 100.0);
        REQUIRE(point_in_window(wind, pt));
    }
    SECTION("point is out of window")
    {
        point_2d pt = point_at(100.1, 100.1);
        REQUIRE_FALSE(point_in_window(wind, pt));
    }
    close_window(wind);
}

TEST_CASE_METHOD(CameraTestFixture, "can move camera by offset", "[move_camera_by]")
{
    move_camera_by(42.0, 100.0);
    REQUIRE_THAT(camera_x(), WithinRel(42.0));
    REQUIRE_THAT(camera_y(), WithinRel(100.0));
}

TEST_CASE_METHOD(CameraTestFixture, "can center camera on sprite", "[center_camera_on]")
{
    open_window("can center camera on sprite", 100, 100);

    // Create 20x20 square sprite at 50,50 
    bitmap square_bmp = create_bitmap("square", 20, 20);
    sprite square_sprt = create_sprite(square_bmp);
    move_sprite_to(square_sprt, 90.0, 90.0);

    SECTION("camera moves to sprite with no offset")
    {
        vector_2d offset = vector_to(0.0, 0.0);
        center_camera_on(square_sprt, offset);
        REQUIRE_THAT(camera_x(), WithinRel(40.0));
        REQUIRE_THAT(camera_y(), WithinRel(40.0));
    }
    SECTION("camera moves to sprite with offset")
    {
        vector_2d offset = vector_to(0.5, 0.5);
        center_camera_on(square_sprt, offset);
        REQUIRE_THAT(camera_x(), WithinRel(40.5));
        REQUIRE_THAT(camera_y(), WithinRel(40.5));
    }
    close_current_window();
}