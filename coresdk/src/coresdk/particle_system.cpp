#include "particle_system.h"
#include <cmath>

// Initialize the particle system
void initialize_particle_system(ParticleSystem &particlesystem, double x, double y)
{
    particlesystem.emitter_x = x;
    particlesystem.emitter_y = y;
    
    particlesystem.default_particle_size = 5.0f; // Default size for particles
    particlesystem.emission_angle = 0.0f;       // Default angle (0 degrees, pointing right)

    particlesystem.last_x = x; // Initialize last position 
    particlesystem.last_y = y; // Initialize last position
    particlesystem.add_streaks=false;
    particlesystem.streak_color=COLOR_WHITE;
    particlesystem.streak_index = 0;
    particlesystem.max_active_particles = 100;       // Default to 100 particles
    particlesystem.decay_velocity = 0.01f;             // Default decay rate
    particlesystem.gravity = 0.0f;                     // Default gravity force
    particlesystem.gravity_enabled = false;           // Gravity disabled by default


    for (int i = 0; i < STREAK_HISTORY; ++i)
    {
        particlesystem.streak_positions[i][0] = x;
        particlesystem.streak_positions[i][1] = y;
    }

    // Initialize all particles as inactive
    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        particlesystem.particles[i].is_active = false;
    }
}

// Spawn a new particle
void spawn_particle(ParticleSystem &particlesystem)
{
    int active_count = 0;

    // Count the number of currently active particles
    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        if (particlesystem.particles[i].is_active)
        {
            active_count++;
        }
    }

    // Spawn new particles only if the active count is less than the user-defined maximum
    if (active_count < particlesystem.max_active_particles)
    {
        int particles_to_spawn = particlesystem.emission_rate;

        for (int i = 0; i < MAX_PARTICLES && particles_to_spawn; ++i)
        {
            if (!particlesystem.particles[i].is_active)
            {
                // Activate and initialize the particle
                particlesystem.particles[i].is_active = true;
                particlesystem.particles[i].x = particlesystem.emitter_x;
                particlesystem.particles[i].y = particlesystem.emitter_y;


                // Randomize angle within spread
                float random_offset = rnd() * particlesystem.spread - (particlesystem.spread / 2.0f); // ±spread/2
                float angle_in_radians = (particlesystem.emission_angle + random_offset) * (PI / 180.0f);

                // Calculate velocity using the randomized angle
                float speed = particlesystem.speed;
                particlesystem.particles[i].dx = cos(angle_in_radians) * speed;
                particlesystem.particles[i].dy = sin(angle_in_radians) * speed;

                particlesystem.particles[i].lifespan = particlesystem.default_particle_lifespan;
                particlesystem.particles[i].size = particlesystem.default_particle_size; // Set the size to the user-defined value
                particlesystem.particles[i].particle_color = particlesystem.default_particle_color;
                break;
            }
        }
    }
}


// Update the particle system
void update_particle_system(ParticleSystem &particlesystem, float delta_time)
{
    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        if (particlesystem.particles[i].is_active)
        {
            // Update particle position
            particlesystem.particles[i].x += particlesystem.particles[i].dx * delta_time * 60; // Normalize for frame rate
            particlesystem.particles[i].y += particlesystem.particles[i].dy * delta_time * 60;

            // Apply decay to velocity
            if (particlesystem.decay_velocity > 0.0f)
            {
                particlesystem.particles[i].dx *= (1.0f - particlesystem.decay_velocity * delta_time);
                particlesystem.particles[i].dy *= (1.0f - particlesystem.decay_velocity * delta_time);
            }

            // Apply gravity
            if (particlesystem.gravity_enabled)
            {
                particlesystem.particles[i].dy += particlesystem.gravity * delta_time * 60;
            }

            // Decrease lifespan
            particlesystem.particles[i].lifespan -= delta_time * 60;

            // Deactivate particle if its lifespan is over
            if (particlesystem.particles[i].lifespan <= 0)
            {
                particlesystem.particles[i].is_active = false;
            }
        }
    }
}


// Render the particle system
void draw_particle_system(const ParticleSystem &particlesystem)
{
    // to add streaks if streaks are enabled
     if (particlesystem.add_streaks)
    {
        for (int i = 0; i < STREAK_HISTORY - 1; ++i)
        {
            // Get the current and next positions from history
            int current_index = (particlesystem.streak_index - i + STREAK_HISTORY) % STREAK_HISTORY;
            int next_index = (particlesystem.streak_index - i - 1 + STREAK_HISTORY) % STREAK_HISTORY;

            float x1 = particlesystem.streak_positions[current_index][0];
            float y1 = particlesystem.streak_positions[current_index][1];
            float x2 = particlesystem.streak_positions[next_index][0];
            float y2 = particlesystem.streak_positions[next_index][1];

            // Calculate the distance between the points
            float dx = x2 - x1;
            float dy = y2 - y1;
            float distance = sqrt(dx * dx + dy * dy);

            // Skip segments with large distances (to prevent the straight line artifact)
            if (distance > 50.0f) // Threshold for skipping outliers
            {
                continue;
            }

            // Determine the number of intermediate circles
            int steps = static_cast<int>(distance / 2.0f); // Smaller value for tighter spacing
            float step_x = dx / (steps > 0 ? steps : 1);
            float step_y = dy / (steps > 0 ? steps : 1);

            for (int j = 0; j <= steps; ++j)
            {
                float segment_x = x1 + step_x * j;
                float segment_y = y1 + step_y * j;

                // Calculate thickness and transparency (thicker and more opaque near the emitter)
                float thickness = 8.0f * (1.0f - static_cast<float>(i) / STREAK_HISTORY);
                int alpha = static_cast<int>(particlesystem.set_streak_transparency * (1.0f - static_cast<float>(i) / STREAK_HISTORY));

                color faded_color = rgba_color(
                    red_of(particlesystem.streak_color),
                    green_of(particlesystem.streak_color),
                    blue_of(particlesystem.streak_color),
                    alpha);

                // Render the circle
                fill_circle(faded_color, segment_x, segment_y, thickness);
            }
        }
    }

    // Rendering the particles
    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
    if (particlesystem.particles[i].is_active)
    {
        // Add Godot-like glow rendering for particles with glow_enabled = true
        if (particlesystem.particles[i].glow_enabled)
        {
            // Create multiple layers of glow to simulate Godot-style glow
            for (int glow_layer = 0; glow_layer < 3; ++glow_layer) // More layers for a smoother gradient
            {
                float glow_size = particlesystem.particles[i].size * (1.5f + glow_layer * 0.8f); // Gradually increase size
                int glow_alpha = static_cast<int>(200 / (glow_layer + 1)); // Stronger inner glow, softer outer layers

                color glow_color = rgba_color(
                    (red_of(particlesystem.particles[i].particle_color) + 255) / 2, // Blend with white
                    (green_of(particlesystem.particles[i].particle_color) + 255) / 2,
                    (blue_of(particlesystem.particles[i].particle_color) + 255) / 2,
                    glow_alpha);

                // Draw the glow layer
                fill_circle(glow_color, particlesystem.particles[i].x, particlesystem.particles[i].y, glow_size);
            }
        }

        // Draw the actual particle
        fill_circle(particlesystem.particles[i].particle_color, particlesystem.particles[i].x, particlesystem.particles[i].y, particlesystem.particles[i].size);
    }
}

}

// for enabling streaks and to set color
void enable_streaks(ParticleSystem &particlesystem, bool add_streaks = false, color streak_color = COLOR_WHITE, int transparency)
{
    particlesystem.add_streaks = add_streaks;
    particlesystem.streak_color = streak_color;
    particlesystem.set_streak_transparency=transparency;
}

void update_streak_history(ParticleSystem &particlesystem)
{
    // Calculate distance between the current emitter position and the last history entry
    float dx = particlesystem.emitter_x - particlesystem.streak_positions[particlesystem.streak_index][0];
    float dy = particlesystem.emitter_y - particlesystem.streak_positions[particlesystem.streak_index][1];
    float distance = sqrt(dx * dx + dy * dy);

    // Update history only if the distance exceeds the threshold
    if (distance > 5.0f) // Threshold for significant movement
    {
        particlesystem.streak_positions[particlesystem.streak_index][0] = particlesystem.emitter_x;
        particlesystem.streak_positions[particlesystem.streak_index][1] = particlesystem.emitter_y;
        particlesystem.streak_index = (particlesystem.streak_index + 1) % STREAK_HISTORY;
    }
}

// helper function to enable glow to individual particles
void enable_particle_glow(ParticleSystem &particlesystem, bool enable = false)
{
    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        particlesystem.particles[i].glow_enabled = enable;
    }
}

// setter function for setting the particle size
void set_particle_size(ParticleSystem &particlesystem, float size)
{
    particlesystem.default_particle_size = size;
}

// setter function for setting the particle lifespan
void set_particle_lifespan(ParticleSystem &particlesystem, int lifespan){
    particlesystem.default_particle_lifespan=lifespan;
}

// setter function for setting the particle emission angle
void set_particle_color(ParticleSystem &particlesystem, color set_color){
    particlesystem.default_particle_color=set_color;
}

// setter function for setting the particle emission angle
void set_emission_angle(ParticleSystem &particlesystem, float angle, float spread=0.0f)
{
    particlesystem.emission_angle = angle; 
    particlesystem.spread=spread; 
}

// setter function for setting the particle emission speed
void set_emission_speed(ParticleSystem &particlesystem, int speed=4){
    particlesystem.speed=speed;
}

// function to set the maximum particles 
void set_max_particles(ParticleSystem &particlesystem, int max_particles)
{
    particlesystem.max_active_particles = max_particles; 
}

// function to set the decay velocity
void set_decay_velocity(ParticleSystem &particlesystem, float decay_rate)
{
    particlesystem.decay_velocity = decay_rate; 
}

// set the gravity
void set_gravity(ParticleSystem &particlesystem, float gravity_force)
{
    particlesystem.gravity = gravity_force;
    particlesystem.gravity_enabled = true; 
}

// function to turn off the gravity 
void disable_gravity(ParticleSystem &particlesystem)
{
    particlesystem.gravity_enabled = false; 
}

// set the emission rate
void set_emission_rate(ParticleSystem &particlesystem, int rate)
{
    particlesystem.emission_rate = rate; // Set the number of particles emitted per frame
}

// getter functions --- 

// for getting the particle size
float get_particle_size(ParticleSystem &particlesystem){
    return particlesystem.default_particle_size;
}

// for getting the particle lifespan
int get_particle_lifespan(ParticleSystem &particlesystem){
    return particlesystem.default_particle_lifespan;
}

// for getting the particle color
color get_particle_color(ParticleSystem &particlesystem){
    return particlesystem.default_particle_color;
}

// for getting the particle emission angle
float get_particle_emission_angle(ParticleSystem &particlesystem){
    return particlesystem.emission_angle;
}

// for getting the particle emission spread
float get_particle_emission_spread(ParticleSystem &particlesystem){
    return particlesystem.spread;
}

// for getting the particle emission speed
int get_emission_speed(ParticleSystem &particlesystem){
    return particlesystem.speed;
}

// for getting the maximum  number of particles
int get_maximum_particles(ParticleSystem &particlesystem){
    return particlesystem.max_active_particles;
}

// for getting the decay velocity rate
float get_decay_velocity(ParticleSystem &particlesystem){
    return particlesystem.decay_velocity;
}

// for getting the gravity value
float get_gravity(ParticleSystem &particlesystem){
    return particlesystem.gravity;
}

// get the emission rate
int get_emission_rate(ParticleSystem &particlesystem){
    return particlesystem.emission_rate;
}