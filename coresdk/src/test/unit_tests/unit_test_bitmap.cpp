/**
 * Bitmap Unit Tests
 */

#include "catch.hpp"

#include "types.h"
#include "graphics.h"
#include "resources.h"
#include "physics.h"
#include "images.h"
#include "rectangle_drawing.h"
#include "color.h"

#include "logging_handling.h"

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

    disable_logging(WARNING); // Disables "WARNING -> Unable to locate file for non_existent"
    bitmap no_bmp = load_bitmap("non_existent", "non_existent.jpg");
    enable_logging(WARNING);
    
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

TEST_CASE("can perform bitmap ray collision detection", "[bitmap][ray_collision][physics]")
{
    // Create opaque bitmaps for testing to avoid transparency issues
    bitmap bmp_1 = create_bitmap("bmp_1", 50, 50);
    clear_bitmap(bmp_1, COLOR_RED);
    bitmap bmp_2 = create_bitmap("bmp_2", 50, 50);
    clear_bitmap(bmp_2, COLOR_BLUE);
    bitmap bmp_3 = create_bitmap("bmp_3", 50, 50);
    clear_bitmap(bmp_3, COLOR_GREEN);

    // Collision tests require pixel masks
    setup_collision_mask(bmp_1);
    setup_collision_mask(bmp_2);
    setup_collision_mask(bmp_3);
    
    REQUIRE(bitmap_valid(bmp_1));
    REQUIRE(bitmap_valid(bmp_2));
    REQUIRE(bitmap_valid(bmp_3));
    
    SECTION("can detect ray collision with bitmap")
    {
        point_2d bmp_position = point_at(100.0, 100.0);
        point_2d ray_origin = point_at(50.0, 125.0);
        vector_2d ray_heading = vector_to(1.0, 0.0);
        
        // Ray should collide with bitmap in its path
        bool collision = bitmap_ray_collision(bmp_1, 0, bmp_position, ray_origin, ray_heading);
        REQUIRE(collision);
        
        // Ray pointing away should not collide
        ray_heading = vector_to(-1.0, 0.0);
        collision = bitmap_ray_collision(bmp_1, 0, bmp_position, ray_origin, ray_heading);
        REQUIRE_FALSE(collision);
    }
    
    SECTION("can detect ray collision with multiple bitmaps at different positions")
    {
        point_2d bmp_1_position = point_at(300.0, 300.0);
        point_2d bmp_2_position = point_at(500.0, 300.0);
        point_2d bmp_3_position = point_at(700.0, 300.0);
        point_2d ray_origin = point_at(100.0, 325.0);

        // Single ray that passes through all three bitmaps
        vector_2d ray_heading = vector_to(1.0, 0.0);
        bool collision_1 = bitmap_ray_collision(bmp_1, 0, bmp_1_position, ray_origin, ray_heading);
        bool collision_2 = bitmap_ray_collision(bmp_2, 0, bmp_2_position, ray_origin, ray_heading);
        bool collision_3 = bitmap_ray_collision(bmp_3, 0, bmp_3_position, ray_origin, ray_heading);
        
        // All should be true as we are using opaque bitmaps
        REQUIRE((collision_1 && collision_2 && collision_3));
    }
    
    SECTION("can detect ray collision with different ray origins")
    {
        point_2d bmp_position = point_at(300.0, 300.0);
        vector_2d ray_heading = vector_to(1.0, 0.0);
        
        // Ray from left should collide
        point_2d ray_origin_left = point_at(200.0, 325.0);
        bool collision_left = bitmap_ray_collision(bmp_1, 0, bmp_position, ray_origin_left, ray_heading);
        REQUIRE(collision_left);
        
        // Ray from far below should not collide
        point_2d ray_origin_below = point_at(200.0, 500.0);
        bool collision_below = bitmap_ray_collision(bmp_1, 0, bmp_position, ray_origin_below, ray_heading);
        REQUIRE_FALSE(collision_below);
    }
    
    SECTION("can handle ray collision with different bitmap cells")
    {
        // Create 2-cell bitmap
        bitmap cell_bmp = create_bitmap("cell_bmp", 100, 50);
        bitmap_set_cell_details(cell_bmp, 50, 50, 2, 1, 2); // w, h, cols, rows, count
        
        // Clear to transparent
        clear_bitmap(cell_bmp, COLOR_TRANSPARENT);
        
        // Draw rect on first cell (left side)
        fill_rectangle_on_bitmap(cell_bmp, COLOR_RED, 0, 0, 50, 50);

        setup_collision_mask(cell_bmp);
        
        point_2d bmp_position = point_at(300.0, 300.0);
        point_2d ray_origin = point_at(200.0, 325.0);
        vector_2d ray_heading = vector_to(1.0, 0.0);
        
        // Test with cell 0 (solid)
        bool collision_cell_0 = bitmap_ray_collision(cell_bmp, 0, bmp_position, ray_origin, ray_heading);
        REQUIRE(collision_cell_0);
        
        // Test with cell 1 (transparent/empty)
        bool collision_cell_1 = bitmap_ray_collision(cell_bmp, 1, bmp_position, ray_origin, ray_heading);
        REQUIRE_FALSE(collision_cell_1);
        
        free_bitmap(cell_bmp);
    }
    
    free_bitmap(bmp_1);
    free_bitmap(bmp_2);
    free_bitmap(bmp_3);
}
