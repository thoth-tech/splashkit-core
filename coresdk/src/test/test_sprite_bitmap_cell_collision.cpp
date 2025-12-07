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
    
    // Load a sprite sheet with multiple frames representing an environmental hazard
    // In this example, we'll create a bitmap representing a spike trap with different states
    // Frame 0: Retracted (no spikes - safe)
    // Frame 1: Extending (medium spikes - some danger)  
    // Frame 2: Fully Extended (large spikes - dangerous)
    bitmap hazard_spritesheet = create_bitmap("spike_trap_sheet", 300, 100);
    
    // Set up cell details: 3 frames (100x100 each) in a horizontal strip
    bitmap_set_cell_details(hazard_spritesheet, 100, 100, 3, 1, 3);
    
    // Frame 0 (Retracted): Small grey square (20x20) at bottom - minimal collision area
    fill_rectangle_on_bitmap(hazard_spritesheet, rgba_color(128, 128, 128, 255), 40, 75, 20, 20);
    
    // Frame 1 (Extending): Medium red trapezoid shape (40x40) - moderate collision area
    // Draw a simple trapezoid to represent extending spikes
    fill_rectangle_on_bitmap(hazard_spritesheet, rgba_color(200, 0, 0, 255), 130, 55, 40, 40);
    fill_triangle_on_bitmap(hazard_spritesheet, rgba_color(200, 0, 0, 255), 
                           130, 55, 170, 55, 150, 40);
    
    // Frame 2 (Extended): Large bright red spike pattern (60x70) - maximum collision area
    // Draw multiple triangular shapes to represent fully extended spikes
    // Base platform
    fill_rectangle_on_bitmap(hazard_spritesheet, rgba_color(150, 0, 0, 255), 220, 75, 60, 20);
    // Spikes pointing up
    fill_triangle_on_bitmap(hazard_spritesheet, rgba_color(255, 0, 0, 255), 
                           220, 75, 230, 75, 225, 25);
    fill_triangle_on_bitmap(hazard_spritesheet, rgba_color(255, 0, 0, 255), 
                           235, 75, 245, 75, 240, 25);
    fill_triangle_on_bitmap(hazard_spritesheet, rgba_color(255, 0, 0, 255), 
                           250, 75, 260, 75, 255, 25);
    fill_triangle_on_bitmap(hazard_spritesheet, rgba_color(255, 0, 0, 255), 
                           265, 75, 275, 75, 270, 25);
    
    // IMPORTANT: Setup collision mask for the hazard bitmap
    // This ensures transparent areas are NOT counted in collision detection
    setup_collision_mask(hazard_spritesheet);
    
    // Create a player sprite with a solid circle
    bitmap player_bmp = create_bitmap("player", 40, 40);
    clear_bitmap(player_bmp, COLOR_TRANSPARENT);
    fill_circle_on_bitmap(player_bmp, COLOR_YELLOW, 20, 20, 18);
    
    // Setup collision mask for player too
    setup_collision_mask(player_bmp);
    
    sprite player = create_sprite("player", player_bmp);
    sprite_set_x(player, 400);
    sprite_set_y(player, 300);
    
    // Hazard bitmap position (fixed on screen)
    double hazard_x = 350;
    double hazard_y = 250;
    
    int current_cell = 0; // Start with retracted spikes
    
    cout << "Sprite Bitmap Cell Collision Test" << endl;
    cout << "Use LEFT/RIGHT arrows to change spike trap animation frame (cell)" << endl;
    cout << "Use WASD to move the player sprite" << endl;
    cout << "Observe how collision changes with different cells!" << endl;
    cout << "The trap cycles through: Retracted -> Extending -> Extended" << endl;
    
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
        
        // Check collision with the SPECIFIC CELL (frame) of the hazard sprite sheet
        // This is the key function we're demonstrating!
        bool colliding = sprite_bitmap_collision(player, hazard_spritesheet, current_cell, hazard_x, hazard_y);
        
        // Draw
        clear_screen(COLOR_WHITE);
        
        // Draw the current cell of the hazard sprite sheet
        drawing_options opts = option_part_bmp(
            current_cell * 100,  // x offset in source
            0,                   // y offset in source
            100,                 // width
            100                  // height
        );
        draw_bitmap(hazard_spritesheet, hazard_x, hazard_y, opts);
        
        // Draw a dashed rectangle around the hazard to show cell boundary
        draw_rectangle(rgba_color(0, 0, 0, 100), hazard_x, hazard_y, 100, 100);
        
        // Draw the player sprite (colour changes based on collision)
        if (colliding)
        {
            // Player is colliding - draw in red with pulsing effect
            fill_circle(rgba_color(255, 0, 0, 200), sprite_x(player) + 20, sprite_y(player) + 20, 20);
            // Draw warning outline
            draw_circle(COLOR_RED, sprite_x(player) + 20, sprite_y(player) + 20, 25);
        }
        else
        {
            // No collision - draw normally
            draw_sprite(player);
        }
        
        // Display information with better formatting
        string cell_names[] = {"Retracted (safe)", "Extending (danger!)", "Extended (deadly!)"};
        color cell_colors[] = {COLOR_GREEN, rgba_color(255, 140, 0, 255), COLOR_RED};
        
        // Title
        draw_text("Spike Trap Cell Collision Demo", COLOR_BLACK, "Arial", 20, 10, 10);
        
        // Current state info
        draw_text("Current Trap State: " + cell_names[current_cell], cell_colors[current_cell], "Arial", 16, 10, 40);
        draw_text("Cell Index: " + to_string(current_cell), COLOR_BLACK, 10, 60);
        
        // Collision status with prominent display
        string collision_text = colliding ? "COLLISION DETECTED!" : "No Collision";
        color collision_color = colliding ? COLOR_RED : COLOR_GREEN;
        draw_text(collision_text, collision_color, "Arial", 18, 10, 90);
        
        // Instructions box
        fill_rectangle(rgba_color(240, 240, 240, 255), 10, 480, 380, 110);
        draw_rectangle(COLOR_BLACK, 10, 480, 380, 110);
        draw_text("Instructions:", COLOR_BLACK, "Arial", 14, 20, 490);
        draw_text("LEFT/RIGHT: Change spike trap animation state", COLOR_BLACK, 20, 510);
        draw_text("WASD: Move player", COLOR_BLACK, 20, 530);
        draw_text("Move close to see collision detection in action!", COLOR_BLACK, 20, 550);
        
        // Explanation box
        fill_rectangle(rgba_color(230, 240, 255, 255), 400, 480, 390, 110);
        draw_rectangle(COLOR_BLUE, 400, 480, 390, 110);
        draw_text("What This Demonstrates:", COLOR_BLUE, "Arial", 14, 410, 490);
        draw_text("Testing collision with SPECIFIC CELLS (frames)", COLOR_BLACK, 410, 510);
        draw_text("Different animation states = different collision areas", COLOR_BLACK, 410, 530);
        draw_text("Notice: Retracted trap has tiny hitbox, extended has large hitbox!", COLOR_BLACK, 410, 550);
        
        refresh_screen(60);
    }
    
    free_sprite(player);
    free_bitmap(player_bmp);
    free_bitmap(hazard_spritesheet);
    close_all_windows();
}
