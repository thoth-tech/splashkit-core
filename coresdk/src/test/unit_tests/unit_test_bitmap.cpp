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

TEST_CASE("bitmaps can be created and freed", "[load_bitmap][bitmap_width][bitmap_height][free_bitmap]")
{
    // Creating bitmaps
    bitmap rocket_bmp, frog_bmp, background_bmp;
    rocket_bmp = load_bitmap("rocket_sprt", "rocket_sprt.png");
    REQUIRE(bitmap_valid(rocket_bmp));
    REQUIRE(rocket_bmp != nullptr);
    REQUIRE(bitmap_width(rocket_bmp) == ROCKET_WIDTH);
    REQUIRE(bitmap_height(rocket_bmp) == ROCKET_HEIGHT);
    frog_bmp = load_bitmap("frog", "frog.png");
    REQUIRE(bitmap_valid(frog_bmp));
    REQUIRE(frog_bmp != nullptr);
    REQUIRE(bitmap_width(frog_bmp) == FROG_WIDTH);
    REQUIRE(bitmap_height(frog_bmp) == FROG_HEIGHT);
    background_bmp = load_bitmap("background", "background.png");
    REQUIRE(bitmap_valid(background_bmp));
    REQUIRE(background_bmp != nullptr);
    REQUIRE(bitmap_width(background_bmp) == BACKGROUND_WIDTH);
    REQUIRE(bitmap_height(background_bmp) == BACKGROUND_HEIGHT);

    // Freeing bitmaps
    free_bitmap(rocket_bmp);
    REQUIRE_FALSE(has_bitmap("rocket_sprt"));
    free_bitmap(frog_bmp);
    REQUIRE_FALSE(has_bitmap("frog"));
    free_bitmap(background_bmp);
    REQUIRE_FALSE(has_bitmap("background"));
}

TEST_CASE("can detect non-existent bitmap")
{
    REQUIRE(has_bitmap("non_existent") == false);
    bitmap no_bmp = load_bitmap("non_existent", "non_existent.jpg");
    REQUIRE(no_bmp == nullptr);
    REQUIRE(has_bitmap("non_existent") == false);
}

TEST_CASE("can load and free multiple bitmaps")
{
    bitmap rocket_bmp, frog_bmp, background_bmp;
    rocket_bmp = load_bitmap("rocket_sprt", "rocket_sprt.png");
    frog_bmp = load_bitmap("frog", "frog.png");
    background_bmp = load_bitmap("background", "background.png");
    REQUIRE(bitmap_valid(rocket_bmp));
    REQUIRE(bitmap_valid(frog_bmp));
    REQUIRE(bitmap_valid(background_bmp));

    // Freeing all bitmaps
    free_all_bitmaps();

    REQUIRE_FALSE(bitmap_valid(rocket_bmp));
    REQUIRE_FALSE(bitmap_valid(frog_bmp));
    REQUIRE_FALSE(bitmap_valid(background_bmp));
}

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

TEST_CASE("name can be retrieved from bitmap", "[bitmap][bitmap_name]")
{   
    SECTION("can get bitmap name")
    {
        bitmap bmp = load_bitmap("rocket", "rocket_sprt.png");
        REQUIRE(bitmap_valid(bmp));
        
        string name = bitmap_name(bmp);
        REQUIRE(name == "rocket");
    }

    SECTION("empty string returned for bitmap name when passing nullptr")
    {
        string name = bitmap_name(nullptr);
        REQUIRE(name == "");
    }

    SECTION("empty string returned for bitmap when passing freed bitmap")
    {
        bitmap bmp = load_bitmap("frog", "frog.png");
        REQUIRE(bitmap_valid(bmp));
        
        free_bitmap(bmp);

        string name = bitmap_name(bmp);
        REQUIRE_FALSE(name == "frog");
        REQUIRE(name == "");
    }

    SECTION("can get unique names from bitmaps when names are reused")
    {
        bitmap bmp_1 = create_bitmap("blank", 300, 200);
        bitmap bmp_2 = create_bitmap("blank", 800, 600);

        string name_1 = bitmap_name(bmp_1);
        string name_2 = bitmap_name(bmp_2);

        REQUIRE(name_1 == "blank");
        REQUIRE(name_2 == "blank0");
    }

    SECTION("can get name of bitmap when name is empty string")
    {
        bitmap bmp = create_bitmap("", 64, 64);
        string name = bitmap_name(bmp);

        REQUIRE(name == "");
    }

    SECTION("can get name of bitmap when name includes special characters")
    {
        string silly_name = "AbCdEf 2+3=5 !@#$% \n";
        bitmap bmp = create_bitmap(silly_name, 128, 128);

        string returned_name = bitmap_name(bmp);
        REQUIRE(returned_name == silly_name);
    }
}
