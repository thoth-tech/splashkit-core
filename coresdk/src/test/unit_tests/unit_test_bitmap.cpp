/**
 * Bitmap Unit Tests
 */

#include "catch.hpp"

#include "types.h"
#include "graphics.h"
#include "resources.h"

using namespace splashkit_lib;

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

// Has Bitmap
TEST_CASE("can determine whether a bitmap exists from a name", "[has_bitmap]")
{
    SECTION("bitmap detected when supplying a valid bitmap name")
    {
        REQUIRE_FALSE(has_bitmap("existent"));
        bitmap bmp = create_bitmap("existent", 32, 32);
        REQUIRE(has_bitmap("existent"));

        free_bitmap(bmp);
    }

    SECTION("bitmap detected when the name is an empty string")
    {
        REQUIRE_FALSE(has_bitmap(""));
        bitmap bmp = create_bitmap("", 32, 32);
        REQUIRE(has_bitmap(""));

        free_bitmap(bmp);
    }
    
    SECTION("no bitmap detected when supplying a non-existent bitmap name")
    {
        REQUIRE_FALSE(has_bitmap("non_existent"));
        
        bitmap bmp = load_bitmap("non_existent", "non_existent.png");
        
        REQUIRE(bmp == nullptr);
        REQUIRE_FALSE(bitmap_valid(bmp));
        
        REQUIRE_FALSE(has_bitmap("non_existent"));

        free_all_bitmaps();
    }
}

// Bitmap Center
TEST_CASE("can get bitmap center coordinates", "[bitmap_center]")
{
    SECTION("can get bitmap center when width and height are even")
    {
        const int width = 128;
        const int height = 64;
        
        bitmap bmp = create_bitmap("sample", width, height);
        
        point_2d center = bitmap_center(bmp);
        REQUIRE(center.x == width / 2.0f);
        REQUIRE(center.y == height / 2.0f);

        free_bitmap(bmp);
    }

    SECTION("can get bitmap center when width and height are odd")
    {
        const int width = 129;
        const int height = 65;
        
        bitmap bmp = create_bitmap("sample", width, height);
        
        point_2d center = bitmap_center(bmp);
        REQUIRE(center.x == width / 2.0f);
        REQUIRE(center.y == height / 2.0f);

        free_bitmap(bmp);
    }

    SECTION("can get bitmap center when width and height are 1")
    {
        bitmap bmp = create_bitmap("sample", 1, 1);
        
        point_2d center = bitmap_center(bmp);
        REQUIRE(center.x == 0.5f);
        REQUIRE(center.y == 0.5f);

        free_bitmap(bmp);
    }

    SECTION("can get bitmap center when width and height are 0")
    {
        bitmap bmp = create_bitmap("sample", 0, 0);
        
        point_2d center = bitmap_center(bmp);
        REQUIRE(center.x == 0.0f);
        REQUIRE(center.y == 0.0f);

        free_bitmap(bmp);
    }
}

// Bitmap Bounding Rectangle
TEST_CASE("can get bitmap bounding rectangle", "[bitmap_bounding_rectangle]")
{
    SECTION("can get bitmap bounding rectangle when width and height are non-zero")
    {
        const int width = 43;
        const int height = 28;
        bitmap bmp = create_bitmap("blank", width, height);
        
        rectangle rect = bitmap_bounding_rectangle(bmp);
        REQUIRE(rect.x == 0.0);
        REQUIRE(rect.y == 0.0);
        REQUIRE(rect.width == width);
        REQUIRE(rect.height == height);

        free_bitmap(bmp);
    }

    SECTION("can get bitmap bounding rectangle when width and height are 1")
    {
        bitmap bmp = create_bitmap("blank", 1, 1);
        
        rectangle rect = bitmap_bounding_rectangle(bmp);
        REQUIRE(rect.x == 0.0);
        REQUIRE(rect.y == 0.0);
        REQUIRE(rect.width == 1);
        REQUIRE(rect.height == 1);

        free_bitmap(bmp);
    }

    SECTION("can get bitmap bounding rectangle when width and height are 0")
    {
        bitmap bmp = create_bitmap("blank", 0, 0);
        
        rectangle rect = bitmap_bounding_rectangle(bmp);
        REQUIRE(rect.x == 0.0);
        REQUIRE(rect.y == 0.0);
        REQUIRE(rect.width == 0);
        REQUIRE(rect.height == 0);

        free_bitmap(bmp);
    }
}

// Bitmap Bounding Circle
TEST_CASE("can get bitmap bounding circle", "[bitmap_bounding_circle]")
{
    const int width = 127;
    const int height = 30;
    
    bitmap bmp = create_bitmap("sample", width, height);
    
    double center_corner_dist = sqrt(pow(width / 2.0, 2.0) + pow(height / 2.0, 2.0));
    
    circle circ = bitmap_bounding_circle(bmp, point_at(100.0, 200.0));
    REQUIRE(circ.center.x == 100.0);
    REQUIRE(circ.center.y == 200.0);
    REQUIRE(circ.radius == center_corner_dist);

    free_bitmap(bmp);
}

// Bitmap Cell Circle
TEST_CASE("can get bitmap cell circle", "[bitmap_cell_circle]")
{
    bitmap bmp = create_bitmap("blank", 256, 256);
    
    bitmap_set_cell_details(bmp, 128, 128, 2, 2, 4);
    
    const double center_corner_dist = sqrt(pow(128 / 2.0, 2.0) + pow(128 / 2.0, 2.0));
    
    point_2d pt = point_at(100.0, 200.0);

    SECTION("can get bitmap cell circle without scale")
    {
        circle circ = bitmap_cell_circle(bmp, pt);
        circle circ2 = bitmap_cell_circle(bmp, pt.x, pt.y);

        REQUIRE(circ.center.x == pt.x);
        REQUIRE(circ.center.y == pt.y);
        REQUIRE(circ.radius == center_corner_dist);

        REQUIRE(circ2.center.x == pt.x);
        REQUIRE(circ2.center.y == pt.y);
        REQUIRE(circ2.radius == center_corner_dist);
    }

    SECTION("can get bitmap cell circle with scale")
    {
        const double scale = 2.0;
        
        circle circ = bitmap_cell_circle(bmp, pt, scale);

        REQUIRE(circ.center.x == pt.x);
        REQUIRE(circ.center.y == pt.y);
        REQUIRE(circ.radius == center_corner_dist * scale);
    }

    free_bitmap(bmp);
}
