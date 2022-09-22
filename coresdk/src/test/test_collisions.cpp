// #include "splashkit.h"
/*
#include "player.h"
#include "obstacle.h"
#include "collisions.h"

/**
 * Load the game images, sounds, etc.
 *\/
void load_resources()
{
    load_resource_bundle("game_bundle", "sprite.txt");
}

/**
 * Entry point.
 * 
 * Manages the initialisation of data, the event loop, and quitting.
 *\/
int run_collisions_test()
{
    open_window("SPRITE_COLLISION_TEST", 1000, 1000);
    load_resources();

    player_data player;
    player = new_player();
    obstacle_data obstacle;
    //parameter '1' is a temporary value, this should be replaced when this testing file is finished
    obstacle = new_obstacle(1);

    while ( not quit_requested() )
    {
        // Handle input to adjust sprites movement
        process_events();
        handle_input(player);
        obstacle_handle_input(obstacle);

        // Perform movement and update the camera
        update_player(player);
        update_obstacle(obstacle, player);

        // Redraw everything
        clear_screen(COLOR_WHITE);

        draw_text("SPRITE_LOCATION: " + point_to_string(center_point(player.player_sprite)), COLOR_BLACK, 0, 10, option_to_screen());
        draw_text("OBSTACLE_LOCATION: " + point_to_string(center_point(obstacle.obstacle_sprite)), COLOR_BLACK, 0, 20, option_to_screen());

        // as well as the player who can move
        draw_player(player);
        draw_obstacle(obstacle);

        //determine whether sprites collision exist and operating further collision reaction

        if(sprite_collision(player.player_sprite,obstacle.obstacle_sprite))
        {
            //value needs to be defined for this
            float SPRITE_KINETIC_ENERGY; 
            if(sprite_rotation(player.player_sprite) >= 0 && sprite_rotation(player.player_sprite) <= 45)
            {
                move_sprite_to(obstacle.obstacle_sprite,sprite_x(obstacle.obstacle_sprite) + SPRITE_KINETIC_ENERGY, sprite_y(obstacle.obstacle_sprite));
                move_sprite_to(player.player_sprite,sprite_x(player.player_sprite) - SPRITE_KINETIC_ENERGY, sprite_y(player.player_sprite));
            }
            if(sprite_rotation(player.player_sprite) > 45 && sprite_rotation(player.player_sprite) <= 135)
            {
                move_sprite_to(obstacle.obstacle_sprite,sprite_x(obstacle.obstacle_sprite),sprite_y(obstacle.obstacle_sprite) + SPRITE_KINETIC_ENERGY);
                move_sprite_to(player.player_sprite,sprite_x(player.player_sprite),sprite_y(player.player_sprite) - SPRITE_KINETIC_ENERGY);
            }
            if(sprite_rotation(player.player_sprite) > 135 && sprite_rotation(player.player_sprite) <= 225)
            {
                move_sprite_to(obstacle.obstacle_sprite,sprite_x(obstacle.obstacle_sprite) - SPRITE_KINETIC_ENERGY, sprite_y(obstacle.obstacle_sprite));
                move_sprite_to(player.player_sprite,sprite_x(player.player_sprite) + SPRITE_KINETIC_ENERGY, sprite_y(player.player_sprite));
            }
            if(sprite_rotation(player.player_sprite) > 225 && sprite_rotation(player.player_sprite) < 315)
            {
                move_sprite_to(obstacle.obstacle_sprite,sprite_x(obstacle.obstacle_sprite),sprite_y(obstacle.obstacle_sprite) - SPRITE_KINETIC_ENERGY);
                move_sprite_to(player.player_sprite,sprite_x(player.player_sprite),sprite_y(player.player_sprite) + SPRITE_KINETIC_ENERGY);
            }
            if(sprite_rotation(player.player_sprite) >= 315 && sprite_rotation(player.player_sprite) <= 360)
            {
                move_sprite_to(obstacle.obstacle_sprite,sprite_x(obstacle.obstacle_sprite) + SPRITE_KINETIC_ENERGY, sprite_y(obstacle.obstacle_sprite));
                move_sprite_to(player.player_sprite,sprite_x(player.player_sprite) - SPRITE_KINETIC_ENERGY, sprite_y(player.player_sprite));
            }
        }
        refresh_screen(60);
    }

    return 0;
}