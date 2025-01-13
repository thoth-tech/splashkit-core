#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include "splashkit.h"

#define MAX_PARTICLES 1000 // Maximum number of particles
#define STREAK_HISTORY 20 // Number of positions to track for the streak
#define PI 3.14 // setting the PI value

// Particle structure to represent individual particle properties
struct Particle
{
    float x, y;           // Position
    float dx, dy;         // Velocity
    float lifespan;       // Remaining lifespan
    float size;           // Particle size
    color particle_color; // Particle color
    bool is_active;       // Active state of the particle
    bool glow_enabled;    // Whether this particle glows
    float weight;         // Particle weight


};

// Particle system to manage all particles
struct ParticleSystem
{
    Particle particles[MAX_PARTICLES]; // Array of particles
    float emitter_x, emitter_y;        // Position of the emitter
    float last_x, last_y;           // stores the last position of the emitter
    bool add_streaks;     // for adding streaks 
    color streak_color;    // for setting the color of the streak
    int set_streak_transparency;       // Transparency of the streak (0 to 255)
    float streak_positions[STREAK_HISTORY][2]; // History of positions (x, y)
    int streak_index;                 // Current index in the streak history
    
    float default_particle_size;       // Default size for new particles
    color default_particle_color;      // Default color for new particles
    int default_particle_lifespan;   // Default lifespan for new particles
    float emission_angle;           // the angle of particle emission
    float spread;                   // to set the spread of the emitted particles
    int speed;                      // to set the speed of the particles emitted
    int max_active_particles;          // Maximum number of active particles
    float decay_velocity;          // rate at which the initial velocity decays
    float gravity;                     // Gravity force applied to particles
    bool gravity_enabled;              // Toggle for gravity
    int emission_rate;                 // Multiplier for particles emitted per frame


};

// Initialize the particle system
void initialize_particle_system(ParticleSystem &particlesystem, double x, double y);

// Spawn a new particle
void spawn_particle(ParticleSystem &particlesystem);

// Update the particle system
void update_particle_system(ParticleSystem &particlesystem, float delta_time);

// Render the particle system
void draw_particle_system(const ParticleSystem &particlesystem);

// add streaks
void enable_streaks(ParticleSystem &particlesystem, bool add_streaks, color streak_color, int transparency=255);

// for updating streak history
void update_streak_history(ParticleSystem &particlesystem);

// helper function to enable individual particle glow
void enable_particle_glow(ParticleSystem &particlesystem, bool enable);

// set functions

// setter function for setting the particle size
void set_particle_size(ParticleSystem &particlesystem, float size);

// setter function for setting the particle lifespan
void set_particle_lifespan(ParticleSystem &particlesystem, int lifespan);

// setter function for setting the  particle color
void set_particle_color(ParticleSystem &particlesystem, color set_color);

// setter function for setting the particle emission angle
void set_emission_angle(ParticleSystem &particlesystem, float angle,float spread);

// setter function for setting the speed at which a particle is emitted
void set_emission_speed(ParticleSystem &particlesystem, int speed);

// function to set the maximum particles 
void set_max_particles(ParticleSystem &particlesystem, int max_particles);

// function to set the decay velocity
void set_decay_velocity(ParticleSystem &particlesystem, float decay_rate);

// set the gravity 
void set_gravity(ParticleSystem &particlesystem, float gravity_force);

// to turn off gravity when needed
void disable_gravity(ParticleSystem &particlesystem);

// set emission rate
void set_emission_rate(ParticleSystem &particlesystem, int rate);

// get functions 

// get the particle size
float get_particle_size(ParticleSystem &particlesystem);

// get the particle lifespan
int get_particle_lifespan(ParticleSystem &particlesystem);

// get the particle color
color get_particle_color(ParticleSystem &particlesystem);

// get the particle emission angle
float get_particle_emission_angle(ParticleSystem &particlesystem);

// get the particle emission spread
float get_particle_emission_spread(ParticleSystem &particlesystem);

// get function for getting the particle emission speed
int get_emission_speed(ParticleSystem &particlesystem);

// function to get the maximum number of particles
int get_maximum_particles(ParticleSystem &particlesystem);

// get the decay rate of the particle
float get_decay_velocity(ParticleSystem &particlesystem);

// get the gravity value 
float get_gravity(ParticleSystem &particlesystem);

// get the emission rate
int get_emission_rate(ParticleSystem &particlesystem);




#endif // PARTICLE_SYSTEM_H
