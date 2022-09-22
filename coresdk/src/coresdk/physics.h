/**
 * @header  physics
 * @author  Andrew Cain
 * @attribute group  physics
 */

#ifndef physics_hpp
#define physics_hpp

#include "matrix_2d.h"
#include "vector_2d.h"
#include "collisions.h"
/*

    void sprite_gravity(sprite s, vector_2d falling, double mass);

    /**
     * Push sprites when collision exist  
     * 
     * @param s1 The first sprite
     * @param s2 The second sprite
     * @param mass1 The mass of first sprite
     * @param mass2 The mass of second sprite
     *\/

    void sprite_push(sprite s1, sprite s2, double mass1, double mass2);

    /**
     * add physics formula including sprite kinetic energy, interaction force and elastic collision into sprite collision and make it more realistic
     *  @param sprite_1 The first sprite
     *  @param sprite_2 The second sprite
     *  @param mass1 The mass of first sprite used for calculate the kinetic energy of first sprite
     *  @param mass2 The mass of second sprite used for calculate the kinetic energy of second sprite
     *  @param speed1 The speed/velocity of first sprite used for calculate the kinetic energy of first sprite
     *  @param speed2 The speed/velocity of second sprite used for calculate the kinetic energy of second sprite
     *\/

    void Physical_sprites_collision(sprite sprite_1, sprite sprite_2, double mass1, double mass2, double speed1, double speed2);

    /**
     * add physics collision function with angles
     *  @param sprite_1 The first sprite
     *  @param sprite_2 The second sprite
     *\/
    void collision_angle(sprite sp1,sprite sp2);

    /**
     * add function for calculating kinetic energy
     *  @param sprite_1 The first sprite
     *\/
    float KINETIC_ENERGY(sprite sprite_1);

    /**
     * add physics collision function with angles
     *  @param sprite_1 The first sprite
     *  @param sprite_2 The second sprite
     *\/
    float velocity_after_collied(sprite sprite_1, sprite sprite_2);



     /**
     * add physics kinetic function for shapes
     *  @param c1_mass        shape mass
     *  @param C1_X_speed     shape speed in x d  
     *  @param C1_Y_speed     shape speed in y d  
     *\/
    float KINETIC_ENERGY(float c1_mass, float C1_X_speed, float C1_Y_speed);


     /**
     * add velocity function for shapes
     *  @param c1_mass        shape1 mass
     *  @param c2_mass        shape2 mass
     *  @param C1_X_speed     shape1 speed in x d  
     *  @param C1_Y_speed     shape1 speed in y d  
     *\/
    float velocity_after_collied(float c1_mass,float c2_mass,float C1_X_speed, float C1_Y_speed);

     /**
     * add collision function for circles
     *  @param c1       circle1
     *  @param c2       circle2
     *  @param c1_mass    shape1 speed in x d  
     *  @param c2_mass     shape1 speed in y d  
     *  @param c1_x_speed    shape1 speed in x d  
     *  @param c2_y_speed     shape1 speed in y d       
     *\/
    void collision(circle c1, circle c2, float c1_mass, float c2_mass, float C1_X_speed, float C1_Y_speed);

     /**
     * add collision function for circles
     *  @param c1       cirlce1
     *  @param c2       circle22
     *  @param c1_mass    shape1 speed in x d  
     *  @param c2_mass     shape1 speed in y d  
     *  @param c1_x_speed    shape1 speed in x d  
     *  @param c2_y_speed     shape1 speed in y d       
     *\/
    void rect_collision(rectangle c1, rectangle c2, float c1_mass, float c2_mass, float C1_X_speed, float C1_Y_speed);
     /**
     * add collision function for circles
     *  @param c1       rectangle1
     *  @param c2       rectangle2
     *  @param c1_mass    shape1 speed in x d  
     *  @param c2_mass     shape1 speed in y d  
     *  @param c1_x_speed    shape1 speed in x d  
     *  @param c2_y_speed     shape1 speed in y d       
     *\/

    float Free_Falling(float ratio, sprite c1, float Gravity);
    /**
     * add function for calculating falling velocity
     *  @param ratio       ratio to calculate falling acceleration
     *  @param c1          sprite 1 
     *  @param Gravity     The gravity   
     *\/
    float density(sprite c1, float volume);
    /**
     * add function for calculating density
     *  @param c1       sprite1
     *  @param volume
     *\/
    float sinking_velocity(sprite c1, float buoyancy, float volume, float Gravity);
    /**
     * add function for calculating sinking velocity
     *  @param c1       
     *  @param buoyancy          
     *  @param volume
     *  @param gravity
     *\/
    void sinking(sprite c1,float Height_of_water, float buoyancy, float Gravity, float volume, float density_of_water);
    /**
     * add function for calculating sinking velocity
     *  @param c1         sprite 1
     *  @param  Height_of_water        
     *  @param  buoyancy
     *  @param  Gravity
     *  @param  volume
     *  @param  density_of_water
     *\/
    float sprite_momentum_x(sprite spr);
    /**
     * Calculates the momentum of a moving sprite in the x-axis
     * @param spr   Sprite object to calculate
     * @return      Returns the momentum of sprite in the x-axis
     *\/
    float sprite_momentum_y(sprite spr);
    /**
     * Calculates the momentum of a moving sprite in the y-axis
     * @param spr   Sprite object to calulcate
     * @return      Returns the momentum of sprite in the x-axis
     *\/
    void sprite_inelastic_collision(sprite spr_a, sprite spr_b);
    /**
     * Calculate, change velocity, and detect collision between two sprites when in inelastic collision
     * @param spr_a Sprite A
     * @param spr_b Sprite B
     *\/
    void sprite_elastic_collision(sprite spr_a, sprite spr_b);
    /**
     * Calculates, change velocities, and detect collision between two sprites when in elastic collision
     * @param spr_a Sprite A
     * @param spr_b Sprite B
     *\/
    void sprite_confine_screen(sprite spr, string screen_name);
    /**
     * Confines the sprite within the playable screen only
     * @param spr         Sprite to confine
     * @param screen_name The name of the window
     *\/
    double sprite_impulse_x(sprite spr);
    /**
     * Calculates and returns the impulse of a sprite from the x-axis.
     * @param spr  Sprite to calculate impulse
     *\/
    double sprite_impulse_y(sprite spr);
    /**
     * Calculates and returns the impulse of a sprite from the x-axis.
     * @param spr  Sprite to calculate impulse
     *\/
    void sprite_set_impulse_x(sprite spr, double impulse);
    /**
     * Sets the sprite's impulse in the x-axis.
     * @param spr       Sprite to set impulse
     * @param impulse   Value to set the sprite's impulse
     *\/
    void sprite_set_impulse_y(sprite spr, double impulse);
    /**
     * Sets the sprite's impulse in the y-axis.
     * @param spr       Sprite to set impulse
     * @param impulse   Value to set the sprite's impulse
     *\/ 
} */
#endif /* physics_hpp */
