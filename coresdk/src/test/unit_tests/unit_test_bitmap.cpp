/**
 * Bitmap Unit Tests
 */

#include "catch.hpp"

#include "types.h"
#include "graphics.h"
#include "resources.h"

using namespace splashkit_lib;

constexpr int ROCKET_WIDTH = 36, ROCKET_HEIGHT = 72,
              BACKGROUND_WIDTH = 864, BACKGROUND_HEIGHT = 769,
              FROG_WIDTH = 294, FROG_HEIGHT = 422;



// Load Bitmap & Free Bitmap
TEST_CASE("bitmaps can be loaded and freed", "[load_bitmap][free_bitmap]")
{
    SECTION("can load and free rocket bitmap")
    {
        bitmap rocket_bmp = load_bitmap("rocket_sprt", "rocket_sprt.png");
        
        REQUIRE_FALSE(rocket_bmp == nullptr);
        REQUIRE(bitmap_valid(rocket_bmp));

        free_bitmap(rocket_bmp);
        REQUIRE_FALSE(has_bitmap("rocket_sprt"));
    }

    SECTION("can load and free frog bitmap")
    {
        bitmap frog_bmp = load_bitmap("frog", "frog.png");

        REQUIRE_FALSE(frog_bmp == nullptr);
        REQUIRE(bitmap_valid(frog_bmp));

        free_bitmap(frog_bmp);
        REQUIRE_FALSE(has_bitmap("frog"));
    }

    SECTION("can load and free background bitmap")
    {
        bitmap background_bmp = load_bitmap("background", "background.png");

        REQUIRE_FALSE(background_bmp == nullptr);
        REQUIRE(bitmap_valid(background_bmp));

        free_bitmap(background_bmp);
        REQUIRE_FALSE(has_bitmap("background"));
    }

    SECTION("can load and free multiple bitmaps at a time")
    {
        bitmap rocket_bmp = load_bitmap("rocket_sprt", "rocket_sprt.png");
        bitmap frog_bmp = load_bitmap("frog", "frog.png");
        bitmap background_bmp = load_bitmap("background", "background.png");
        
        REQUIRE(bitmap_valid(rocket_bmp));
        REQUIRE(bitmap_valid(frog_bmp));
        REQUIRE(bitmap_valid(background_bmp));

        free_all_bitmaps();

        REQUIRE_FALSE(bitmap_valid(rocket_bmp));
        REQUIRE_FALSE(bitmap_valid(frog_bmp));
        REQUIRE_FALSE(bitmap_valid(background_bmp));
    }
}

// Bitmap Width & Bitmap Height
TEST_CASE("width and height can be retrieved from bitmap", "[bitmap_width][bitmap_height]")
{
    SECTION("can get width and height from bitmap")
    {
        int width = 256;
        int height = 128;
        bitmap bmp = create_bitmap("blank", width, height);

        REQUIRE(bitmap_width(bmp) == width);
        REQUIRE(bitmap_height(bmp) == height);

        REQUIRE(bitmap_width("blank") == width);
        REQUIRE(bitmap_height("blank") == height);

        free_bitmap(bmp);
    }

    SECTION("can get width and height from bitmap when zero")
    {
        bitmap bmp = create_bitmap("blank", 0, 0);

        REQUIRE(bitmap_width(bmp) == 0);
        REQUIRE(bitmap_height(bmp) == 0);

        REQUIRE(bitmap_width("blank") == 0);
        REQUIRE(bitmap_height("blank") == 0);

        free_bitmap(bmp);
    }

    SECTION("can get width and height from bitmap when negative")
    {
        int width = -1024;
        int height = -512;
        bitmap bmp = create_bitmap("blank", width, height);

        REQUIRE(bitmap_width(bmp) == width);
        REQUIRE(bitmap_height(bmp) == height);

        REQUIRE(bitmap_width("blank") == width);
        REQUIRE(bitmap_height("blank") == height);

        free_bitmap(bmp);
    }

    SECTION("zero returned for width and height when passing empty bitmap")
    {
        bitmap bmp;
        REQUIRE(bitmap_width(bmp) == 0);
        REQUIRE(bitmap_height(bmp) == 0);
    }

    SECTION("zero returned for width and height when passing freed bitmap")
    {
        int width = 256;
        int height = 64;
        bitmap bmp = create_bitmap("blank", width, height);
        free_bitmap(bmp);
        
        REQUIRE_FALSE(bitmap_width(bmp) == width);
        REQUIRE_FALSE(bitmap_height(bmp) == height);

        REQUIRE(bitmap_width(bmp) == 0);
        REQUIRE(bitmap_height(bmp) == 0);
    }

    SECTION("zero returned for width and height when passing nullptr as bitmap")
    {
        REQUIRE(bitmap_width(nullptr) == 0);
        REQUIRE(bitmap_height(nullptr) == 0);
    }
}

// TODO: Refactor
TEST_CASE("can detect non-existent bitmap")
{
    REQUIRE(has_bitmap("non_existent") == false);
    bitmap no_bmp = load_bitmap("non_existent", "non_existent.jpg");
    REQUIRE(no_bmp == nullptr);
    REQUIRE(has_bitmap("non_existent") == false);
}

// TODO: Refactor
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
