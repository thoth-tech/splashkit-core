//
//  physics.cpp
//  splashkit
//
//  Created by Clancy Light Townsend on 18/08/2016.
//  Copyright © 2016 Andrew Cain. All rights reserved.
//

#include "physics.h"

/*

    void sprite_gravity(sprite s, vector_2d falling, double mass)
    {
        sprite_set_velocity(s, falling);
        sprite_set_mass(s, mass);
    }

    void sprite_push(sprite s1, sprite s2, double mass1, double mass2)
    {
        if (mass1 > mass2)
        {
            sprite_set_velocity(s2, vector_from_angle(angle_between(sprite_velocity(s2), sprite_velocity(s1)), mass1 - mass2));
        }
        else if (mass1 = mass2 || mass1 < mass2)
        {
            sprite_set_velocity(s2, vector_from_angle(angle_between(sprite_velocity(s2), sprite_velocity(s1)), 0));
            sprite_set_velocity(s1, vector_from_angle(-angle_between(sprite_velocity(s2), sprite_velocity(s1)), 0.1));
        }
    }

    // Insert physics formula into sprite collision, make sprite collsion more realistic
    void Physical_sprites_collision(sprite sprite_1, sprite sprite_2, double mass1, double mass2, double speed1, double speed2)
    {
        double sprite1_kinetic_energy = 0.5 * mass1 * speed1 * speed1;
        double sprite2_kinetic_energy = 0.5 * mass2 * speed2 * speed2;
        double sprite_kinetic_energy = sprite1_kinetic_energy - sprite2_kinetic_energy;
        if (sprite_collision(sprite_1, sprite_2))
        {
            if (sprite_rotation(sprite_1) >= 0 && sprite_rotation(sprite_1) <= 45)
            {
                move_sprite_to(sprite_2, sprite_x(sprite_2) + sprite_kinetic_energy, sprite_y(sprite_2));
                move_sprite_to(sprite_1, sprite_x(sprite_1) - sprite_kinetic_energy, sprite_y(sprite_1));
            }
            if (sprite_rotation(sprite_1) > 45 && sprite_rotation(sprite_1) <= 135)
            {
                move_sprite_to(sprite_2, sprite_x(sprite_2), sprite_y(sprite_2) + sprite_kinetic_energy);
                move_sprite_to(sprite_1, sprite_x(sprite_1), sprite_y(sprite_1) - sprite_kinetic_energy);
            }
            if (sprite_rotation(sprite_1) > 135 && sprite_rotation(sprite_1) <= 225)
            {
                move_sprite_to(sprite_2, sprite_x(sprite_2) - sprite_kinetic_energy, sprite_y(sprite_2));
                move_sprite_to(sprite_1, sprite_x(sprite_1) + sprite_kinetic_energy, sprite_y(sprite_1));
            }
            if (sprite_rotation(sprite_1) > 225 && sprite_rotation(sprite_1) < 315)
            {
                move_sprite_to(sprite_2, sprite_x(sprite_2), sprite_y(sprite_2) - sprite_kinetic_energy);
                move_sprite_to(sprite_1, sprite_x(sprite_1), sprite_y(sprite_1) + sprite_kinetic_energy);
            }
            if (sprite_rotation(sprite_1) >= 315 && sprite_rotation(sprite_1) <= 360)
            {
                move_sprite_to(sprite_2, sprite_x(sprite_2) + sprite_kinetic_energy, sprite_y(sprite_2));
                move_sprite_to(sprite_1, sprite_x(sprite_1) - sprite_kinetic_energy, sprite_y(sprite_1));
            }
        }
    }

    // Function used for calculating kinetic energy
    float KINETIC_ENERGY(sprite sprite_1)
    {
        float KINETIC;
        // KINETIC_ENERGY = 1/2 m*v*v
        KINETIC = 0.5 * sprite_mass(sprite_1) * sprite_speed(sprite_1) * sprite_speed(sprite_1);
        return KINETIC;
    }

    float velocity_after_collied(sprite sprite_1, sprite sprite_2)
    {
        float velocity;
        // According to the law of energy conservation, the Obstacle will gain all kinetic energy from
        // player, and ENERGY = 1/2 m*v*v, we have already know the mass of obstacle, so
        // v = root of (2*ENERGY/m)
        //
        velocity = sqrt(2 * KINETIC_ENERGY(sprite_1) / sprite_mass(sprite_2));
        return velocity;
    }

    // Partial collision with angles function
    void collision_angle(sprite sp1, sprite sp2)
    {
        float velocity;
        velocity = velocity_after_collied(sp1, sp2);

        if (sprite_collision(sp1, sp2))
        {
            // Assume when sprite collied with obstacle, sprite will transfer its all kinetic energy to obstacle, so sprite will stop moving.
            sprite_set_dx(sp1, 0);

            if (sprite_rotation(sp1) >= 0 && sprite_rotation(sp1) <= 45)
            {
                move_sprite_to(sp2, sprite_x(sp2) + velocity, sprite_y(sp2));
                sprite_set_dx(sp2, velocity);
            }
            if (sprite_rotation(sp1) > 45 && sprite_rotation(sp1) <= 135)
            {
                move_sprite_to(sp2, sprite_x(sp2), sprite_y(sp2) + velocity);
                sprite_set_dy(sp2, velocity);
            }
            if (sprite_rotation(sp1) > 135 && sprite_rotation(sp1) <= 225)
            {
                move_sprite_to(sp2, sprite_x(sp2) - velocity, sprite_y(sp2));
                sprite_set_dx(sp2, -velocity);
            }
            if (sprite_rotation(sp1) > 225 && sprite_rotation(sp1) < 315)
            {
                move_sprite_to(sp2, sprite_x(sp2), sprite_y(sp2) - velocity);
                sprite_set_dy(sp2, -velocity);
            }
            if (sprite_rotation(sp1) >= 315 && sprite_rotation(sp1) <= 360)
            {
                move_sprite_to(sp2, sprite_x(sp2) + velocity, sprite_y(sp2));
                sprite_set_dx(sp2, velocity);
            }
        }
        if (GetKeyState('S'))
        {
            sprite_set_dx(sp2, 0);
            sprite_set_dy(sp2, 0);
        }
    }

    // Shape kinetic energy and velocity
    float KINETIC_ENERGY(float c1_mass, float C1_X_speed, float C1_Y_speed)
    {
        float speed;
        float KINETIC;
        speed = sqrt(C1_X_speed * C1_X_speed + C1_Y_speed * C1_Y_speed);
        // KINETIC_ENERGY = 1/2 m*v*v
        KINETIC = 0.5 * c1_mass * speed * speed;
        return KINETIC;
    }

    float velocity_after_collied(float c1_mass, float c2_mass, float C1_X_speed, float C1_Y_speed)
    {
        float velocity;
        //According to the law of energy conservation, the Obstacle will gain all kinetic energy from
        //player, and ENERGY = 1/2 m*v*v, we have already know the mass of obstacle, so
        //v = root of (2*ENERGY/m)
        velocity = sqrt(2 * KINETIC_ENERGY(c1_mass, C1_X_speed, C1_Y_speed) / c2_mass);
        return velocity;
    }

    // Circle collision perform
    void collision(circle c1, circle c2, float c1_mass, float c2_mass, float C1_X_speed, float C1_Y_speed)
    {
        float Obstacle_velocity;
        float circle_X_speed;
        float circle_Y_speed;
        float obstacle_X_speed;
        float obstacle_Y_speed;

        Obstacle_velocity = velocity_after_collied(c1_mass, c2_mass, C1_X_speed, C1_Y_speed);

        

        if (circles_intersect(c1, c2))
        {
            circle_X_speed = 0;
            circle_Y_speed = 0;
            if (Obstacle_velocity < 0.1)
            {
                Obstacle_velocity = 0;
            }

            if (circle_x(c1) < circle_x(c2))
            {
                obstacle_X_speed += Obstacle_velocity;
            }
            if (circle_x(c1) > circle_x(c2))
            {
                obstacle_X_speed -= Obstacle_velocity;
            }
            if (circle_y(c1) > circle_y(c2))
            {
                obstacle_Y_speed -= Obstacle_velocity;
            }
            if (circle_y(c1) < circle_y(c2))
            {
                obstacle_Y_speed += Obstacle_velocity;
            }
        }
    }

    // Rectangle collsion
    void rect_collision(rectangle c1, rectangle c2, float c1_mass, float c2_mass, float C1_X_speed, float C1_Y_speed)
    {
        float Obstacle_velocity;
        float circle_X_speed;
        float circle_Y_speed;
        float obstacle_X_speed;
        float obstacle_Y_speed;

        Obstacle_velocity = velocity_after_collied(c1_mass, c2_mass, C1_X_speed, C1_Y_speed);

        if (rectangles_intersect(c1, c2))
        {
            circle_X_speed = 0;
            circle_Y_speed = 0;
            if (Obstacle_velocity < 0.1)
            {
                Obstacle_velocity = 0;
            }

            if (rectangle_left(c1) < rectangle_left(c2))
            {
                obstacle_X_speed += Obstacle_velocity;
            }
            else if (rectangle_left(c1) > rectangle_left(c2))
            {
                obstacle_X_speed -= Obstacle_velocity;
            }
            else if (rectangle_left(c1) > rectangle_left(c2))
            {
                obstacle_Y_speed -= Obstacle_velocity;
            }
            else if (rectangle_left(c1) < rectangle_left(c2))
            {
                obstacle_Y_speed += Obstacle_velocity;
            }
        }
    }

    // Calculate free falling velocity
    float Free_Falling(float ratio, sprite c1, float Gravity)
    {
        float acceleration;
        float falling_velocity;
        // Gravity
        acceleration = Gravity * ratio;
        falling_velocity = acceleration + sprite_dy(c1);

        return falling_velocity;
    }

    // Calculate density
    float density(sprite c1, float volume)
    {
        float density;
        density = sprite_mass(c1) / volume;

        return density;
    }

    // Calculate sinking velocity.
    float sinking_velocity(sprite c1, float buoyancy, float volume, float Gravity)
    {
        float velocity;
        float F = sprite_mass(c1) * sprite_speed(c1);
        float density = sprite_mass(c1) / volume;
        float F_after_water = sqrt((F - buoyancy * density) * (F - buoyancy * density));

        velocity = F_after_water / sprite_mass(c1);

        return velocity;
    }

    // Simulating sinking effects.
    void sinking(sprite c1, float Height_of_water, float buoyancy, float Gravity, float volume, float density_of_water)
    {
        if (sprite_y(c1) > Height_of_water && density(c1, volume) > density_of_water)
        {
            sprite_set_dy(c1, sinking_velocity(c1, buoyancy, volume, Gravity));
        }
        if (sprite_y(c1) > Height_of_water && density(c1, volume) < density_of_water)
        {
            sprite_set_dy(c1, 0);
        }
    }

    // Calculate the momentum of a moving sprite.
    float sprite_momentum_x(sprite spr)
    {
        return sprite_mass(spr) * sprite_velocity(spr).x;
    }
    float sprite_momentum_y(sprite spr)
    {
        return sprite_mass(spr) * sprite_velocity(spr).y;
    }

    // Formula to calculate and change the final velocity of both objects experiencing an inelastic collision.
    void sprite_inelastic_collision(sprite spr_a, sprite spr_b)
    {
        float mass_a, mass_b, v_a, v_b, v_final;

        // The mass of two sprites
        mass_a = sprite_mass(spr_a);
        mass_b = sprite_mass(spr_b);

        // The velocities of two sprites
        v_a = sprite_velocity(spr_a).x;
        v_b = sprite_velocity(spr_b).x;

        // Formula
        v_final = ((mass_a * v_a) + (mass_b * v_b)) / (mass_a + mass_b);

        // Set the new velocities of the sprites after the calculations
        if (sprite_collision(spr_a, spr_b))
        {
            sprite_set_dx(spr_a, v_final);
            sprite_set_dx(spr_b, v_final);
        }
    }

    // Formula to calculate and change the final velocity of both objects experiencing an elastic collision.
    void sprite_elastic_collision(sprite spr_a, sprite spr_b)
    {
        float mass_a, mass_b, v_a, v_b, v_a_final, v_b_final;

        // The mass of two sprites
        mass_a = sprite_mass(spr_a);
        mass_b = sprite_mass(spr_b);

        // The velocities of two sprites
        v_a = sprite_velocity(spr_a).x;
        v_b = sprite_velocity(spr_b).x;

        // Formula
        v_a_final = (((mass_a - mass_b) / (mass_a + mass_b)) * v_a + ((2 * mass_b) / (mass_a + mass_b)) * v_b);
        v_b_final = (((2 * mass_a) / (mass_a + mass_b)) * v_a + ((mass_b - mass_a) / (mass_a + mass_b)) * v_b);

        // Set the new velocities of the sprite after the calculations
        if (sprite_collision(spr_a, spr_b))
        {
            sprite_set_dx(spr_a, v_a_final);
            sprite_set_dx(spr_b, v_b_final);
        }
    }

    // This function will confine the sprite within the screen.
    void sprite_confine_screen(sprite spr, string screen_name)
    {
        // These series of if statements prevent the sprite from moving away from the screen.
        if ((sprite_x(spr) + sprite_width(spr)) > window_width(screen_name))
        {
            sprite_set_x(spr, window_width(screen_name) - sprite_width(spr));
        }
        if (sprite_x(spr) < 0)
        {
            sprite_set_x(spr, 0);
        }
        if (sprite_y(spr) < 0)
        {
            sprite_set_y(spr, 0);
        }
        if ((sprite_y(spr) + sprite_height(spr)) > window_height(screen_name))
        {
            sprite_set_y(spr, window_height(screen_name) - sprite_height(spr));
        }
    }

    // This function will return the sprite's impulse from the x-axis.
    double sprite_impulse_x(sprite spr)
    {
        // i = f*t = m*v
        double impulse = sprite_mass(spr) * sprite_velocity(spr).x;
        return impulse;
    }

    // This function will return the sprite's impulse from the y-axis.
    double sprite_impulse_y(sprite spr)
    {
        // i = f*t = m*v
        double impulse = sprite_mass(spr) * sprite_velocity(spr).y;
        return impulse;
    }

    // This function will set the sprite's impulse in the x-axis.
    void sprite_set_impulse_x(sprite spr, double impulse)
    {
        // v = i / m
        double final_velocity_x = impulse / sprite_mass(spr);
        sprite_set_dx(spr, final_velocity_x);
    }

    // This function will set the sprite's impulse in the y-axis.
    void sprite_set_impulse_y(sprite spr, double impulse)
    {
        // v = i / m
        double final_velocity_y = impulse / sprite_mass(spr);
        sprite_set_dy(spr, final_velocity_y);
    }
*/

