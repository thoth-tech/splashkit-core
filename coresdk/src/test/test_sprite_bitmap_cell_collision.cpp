//
//  test_sprite_bitmap_cell_collision.cpp
//  splashkit
//
//  Example demonstrating sprite-bitmap collision detection using cells.
//  This shows how to check collision against specific animation frames
//  within a sprite sheet rather than the entire bitmap.
//

#include "sprites.h"
#include "images.h"
#include "graphics.h"
#include "window_manager.h"
#include "input.h"
#include "collisions.h"
#include "text.h"
#include <iostream>

using namespace std;
using namespace splashkit_lib;

void run_sprite_bitmap_cell_collision_test()
{
    open_window("Sprite Bitmap Cell Collision Test", 800, 600);
    
    // Load a sprite sheet with multiple frames
    // In this example, we'll create a simple bitmap with cells representing
    // different animation states (e.g., idle, walk, attack)
    bitmap enemy_spritesheet = create_bitmap("enemy_sheet", 300, 100);
    
    // Set up cell details: 3 frames (100x100 each) in a horizontal strip
    // Frame 0: Idle (small collision area)
    // Frame 1: Walking (medium collision area)  
    // Frame 2: Attacking (large collision area)
    bitmap_set_cell_details(enemy_spritesheet, 100, 100, 3, 1, 3);
    
    // Draw different coloured rectangles in each cell to represent different states
    // Note: In a real game, you'd load an actual sprite sheet image
    clear_bitmap(enemy_spritesheet, COLOR_TRANSPARENT);
    
    // Frame 0 (Idle): Small red square (30x30) centred in the cell
    fill_rectangle_on_bitmap(enemy_spritesheet, COLOR_RED, 35, 35, 30, 30);
    
    // Frame 1 (Walking): Medium green square (50x50) centred in the cell
    fill_rectangle_on_bitmap(enemy_spritesheet, COLOR_GREEN, 125, 25, 50, 50);
    
    // Frame 2 (Attacking): Large blue square (70x70) centred in the cell
    fill_rectangle_on_bitmap(enemy_spritesheet, COLOR_BLUE, 215, 15, 70, 70);
    
    setup_collision_mask(enemy_spritesheet);
    
    // Create a player sprite
    bitmap player_bmp = create_bitmap("player", 40, 40);
    fill_rectangle_on_bitmap(player_bmp, COLOR_YELLOW, 0, 0, 40, 40);
    setup_collision_mask(player_bmp);
    
    sprite player = create_sprite("player", player_bmp);
    sprite_set_x(player, 400);
    sprite_set_y(player, 300);
    
    // Enemy bitmap position (we'll check collision with different cells)
    double enemy_x = 400;
    double enemy_y = 100;
    
    int current_cell = 0; // Start with idle frame
    
    cout << "Sprite Bitmap Cell Collision Test" << endl;
    cout << "Use LEFT/RIGHT arrows to change enemy animation frame (cell)" << endl;
    cout << "Use WASD to move the player sprite" << endl;
    cout << "Observe how collision changes with different cells!" << endl;
    
    while (!quit_requested())
    {
        process_events();
        
        // Handle input for changing cells
        if (key_typed(RIGHT_KEY))
        {
            current_cell = (current_cell + 1) % 3;
        }
        if (key_typed(LEFT_KEY))
        {
            current_cell = (current_cell - 1 + 3) % 3;
        }
        
        // Move player with WASD
        if (key_down(W_KEY)) sprite_set_y(player, sprite_y(player) - 3);
        if (key_down(S_KEY)) sprite_set_y(player, sprite_y(player) + 3);
        if (key_down(A_KEY)) sprite_set_x(player, sprite_x(player) - 3);
        if (key_down(D_KEY)) sprite_set_x(player, sprite_x(player) + 3);
        
        // Check collision with the SPECIFIC CELL (frame) of the enemy sprite sheet
        // This is the key function we're demonstrating!
        bool colliding = sprite_bitmap_collision(player, enemy_spritesheet, current_cell, enemy_x, enemy_y);
        
        // Also demonstrate the point-based version
        point_2d enemy_pt = point_at(enemy_x, enemy_y);
        bool colliding_pt = sprite_bitmap_collision(player, enemy_spritesheet, current_cell, enemy_pt);
        
        // Draw
        clear_screen(COLOR_WHITE);
        
        // Draw the current cell of the enemy sprite sheet
        drawing_options opts = option_part_bmp(
            current_cell * 100,  // x offset in source
            0,                   // y offset in source
            100,                 // width
            100                  // height
        );
        draw_bitmap(enemy_spritesheet, enemy_x, enemy_y, opts);
        
        // Draw a rectangle around the enemy to show cell boundary
        draw_rectangle(COLOR_BLACK, enemy_x, enemy_y, 100, 100);
        
        // Draw the player sprite (colour changes based on collision)
        if (colliding)
        {
            // Player is colliding - draw in red
            fill_circle(COLOR_RED, sprite_x(player) + 20, sprite_y(player) + 20, 25);
        }
        else
        {
            // No collision - draw normally
            draw_sprite(player);
        }
        
        // Display information
        string cell_names[] = {"Idle (small)", "Walking (medium)", "Attacking (large)"};
        draw_text("Current Enemy Frame: " + cell_names[current_cell], COLOR_BLACK, 10, 10);
        draw_text("Cell Index: " + to_string(current_cell), COLOR_BLACK, 10, 30);
        draw_text("Collision Detected: " + string(colliding ? "YES" : "NO"), 
                  colliding ? COLOR_RED : COLOR_GREEN, 10, 50);
        
        draw_text("Instructions:", COLOR_BLACK, 10, 500);
        draw_text("LEFT/RIGHT: Change enemy animation frame", COLOR_BLACK, 10, 520);
        draw_text("WASD: Move player", COLOR_BLACK, 10, 540);
        
        // Explain the concept
        draw_text("This demonstrates checking collision with SPECIFIC CELLS (frames)", 
                  COLOR_BLUE, 10, 450);
        draw_text("Different animation frames have different collision areas!", 
                  COLOR_BLUE, 10, 470);
        
        refresh_screen(60);
    }
    
    free_sprite(player);
    free_bitmap(player_bmp);
    free_bitmap(enemy_spritesheet);
    close_all_windows();
}
