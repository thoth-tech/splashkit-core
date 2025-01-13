/*
A showcase demonstrating various functions in the particle system
*/

#include "splashkit.h"
#include "particle_system.h"

int main()
{
    // Open a new window for rendering
    open_window("Particle System Test", 1000, 800);

    // Create multiple particle systems for demonstration
    ParticleSystem ps1, ps2, ps3, ps4, ps5, ps6;

    // Initialize particle systems at different positions
    initialize_particle_system(ps1, 100, 100); // Top-left corner
    initialize_particle_system(ps2, 300, 100); // Top-center
    initialize_particle_system(ps3, 500, 100); // Top-right corner
    initialize_particle_system(ps4, 100, 400); // Bottom-left corner
    initialize_particle_system(ps5, 300, 400); // Bottom-center
    initialize_particle_system(ps6, 500, 400); // Bottom-right corner

    // Configure particle systems
    // System 1: Basic particles
    set_emission_rate(ps1, 5);
    set_particle_lifespan(ps1, 100);
    set_particle_size(ps1, 5);
    set_particle_color(ps1, COLOR_GREEN);
    set_emission_angle(ps1, 0, 180); // Half-circle emission
    set_emission_speed(ps1, 3);

    // System 2: Fast particles with a wide spread
    set_emission_rate(ps2, 10);
    set_particle_lifespan(ps2, 50);
    set_particle_size(ps2, 8);
    set_particle_color(ps2, COLOR_BLUE);
    set_emission_angle(ps2, 90, 90); // Narrow spread upwards
    set_emission_speed(ps2, 6);

    // System 3: Large slow particles
    set_emission_rate(ps3, 3);
    set_particle_lifespan(ps3, 200);
    set_particle_size(ps3, 15);
    set_particle_color(ps3, COLOR_YELLOW);
    set_emission_angle(ps3, 270, 30); // Downward emission
    set_emission_speed(ps3, 2);

    // System 4: Rotating emitter
    set_emission_rate(ps4, 5);
    set_particle_lifespan(ps4, 120);
    set_particle_size(ps4, 6);
    set_particle_color(ps4, COLOR_RED);
    set_emission_angle(ps4, 0, 360); // Full-circle emission
    set_emission_speed(ps4, 4);

    // System 5: Particles with decay
    set_emission_rate(ps5, 8);
    set_particle_lifespan(ps5, 150);
    set_particle_size(ps5, 10);
    set_particle_color(ps5, COLOR_PURPLE);
    set_emission_angle(ps5, 45, 45); // Diagonal spread
    set_emission_speed(ps5, 5);
    set_decay_velocity(ps5, 0.02f);

    // System 6: Gravity-enabled particles
    set_emission_rate(ps6, 6);
    set_particle_lifespan(ps6, 100);
    set_particle_size(ps6, 7);
    set_particle_color(ps6, COLOR_ORANGE);
    set_emission_angle(ps6, 90, 60); // Upwards with spread
    set_emission_speed(ps6, 4);
    set_gravity(ps6, 0.1f);

    // Game loop
    while (!window_close_requested("Particle System Test"))
    {
        process_events();

        // Clear the screen
        clear_screen(COLOR_BLACK);

        // Update all particle systems
        update_particle_system(ps1, 1.0f / 60.0f);
        update_particle_system(ps2, 1.0f / 60.0f);
        update_particle_system(ps3, 1.0f / 60.0f);
        update_particle_system(ps4, 1.0f / 60.0f);
        update_particle_system(ps5, 1.0f / 60.0f);
        update_particle_system(ps6, 1.0f / 60.0f);

        // Spawn particles for all systems
        spawn_particle(ps1);
        spawn_particle(ps2);
        spawn_particle(ps3);
        spawn_particle(ps4);
        spawn_particle(ps5);
        spawn_particle(ps6);

        // Render all particle systems
        draw_particle_system(ps1);
        draw_particle_system(ps2);
        draw_particle_system(ps3);
        draw_particle_system(ps4);
        draw_particle_system(ps5);
        draw_particle_system(ps6);

        // Refresh the screen
        refresh_screen(60); // 60 FPS
    }

    // Close the window
    close_window("Particle System Test");

    return 0;
}
