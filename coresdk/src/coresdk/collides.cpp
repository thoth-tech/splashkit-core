//
//  collides.cpp
//  splashkit
//
//  Created by RO 01/09/2023.
//  
//  Pascal original comments marked by "--"
//

#include "collides.h"
#include "sprites.h"
#include "utility_functions.h"
#include "graphics.h"

using std::function;

namespace splashkit_lib
{
    
    //
    // this function appears to be used to reflect the velocity of the sprite along the line normal
    //
    void _collide_circle_line(sprite s, const line &line_segment)
    {
        float np_x, np_y, dot_pod;
        vector_2d to_line, s_vel;
        point_2d intersect;
        
        s_vel = sprite_velocity(s);

        //-- TODO: fix collision pt..... cast  back along velocity...
        // gets line point closest to sprite center
        intersect = closest_point_on_line(center_point(s), line_segment);           

        //draw_sprite(s);
        //draw_circle(COLOR_RED, intersect.x, intersect.y, 2);

        // gets unit vector from sprite center to closest point
        to_line = unit_vector(vector_from_center_sprite_to_point(s, intersect));    

        //-- Project velocity across to-line
        // gets velocity magnitude towards closest point
        dot_pod = dot_product(to_line, s_vel);                                      

        // get sprite velocity towards closest point
        np_x = dot_pod * to_line.x;                                                 
        np_y = dot_pod * to_line.y;                                                 

        // reverse the velocity towards closest point. Pascal source used addition, this has been inverted.
        // 2x value, because need to remove original velocity as well
        s_vel.x -= 2 * np_x;                                                        
        s_vel.y -= 2 * np_y;
        sprite_set_velocity(s, s_vel);

        //draw_line(COLOR_YELLOW, center_point(s).x, center_point(s).y, center_point(s).x + (s_vel.x * 10), center_point(s).y + (s_vel.y * 10));
        //refresh_screen(1);

    }

    // function from vector_2d.cpp
    vector_2d vector_over_lines_from_circle(const circle &c, const vector<line> lines, const vector_2d &velocity, int &max_idx);
    
    void collide_circle_lines (sprite s, const vector<line> lines)
    {
        vector_2d out_vec, mvmt;
        int max_idx;
        float mvmt_mag, prop;
        
        if (s == NULL)                      
            return;

        mvmt = sprite_velocity(s);
        max_idx = -1;
        out_vec = vector_over_lines_from_circle(sprite_collision_circle(s), lines, mvmt, max_idx);

        // collide_circle_rectangle uses "hit_idx", and == -1 check, for some reason
        if (max_idx < 0)
            return;

        move_sprite(s,out_vec);
        _collide_circle_line(s, lines[max_idx]);

        //-- do part velocity
        mvmt_mag = vector_magnitude(mvmt);
        prop = vector_magnitude(out_vec) / mvmt_mag;    //-- proportion of move "undone" by back out


        if (prop > 0)                   //-- TODO: Allow proportion of move to be passed in (overload)... then do velocity based on prop * pct
            move_sprite(s,prop);
        

    }

    void collide_circle_line(sprite s, const line &line_segment)
    {
        vector<line> lines = vector<line>(1);
        lines[0] = line_segment;

        collide_circle_lines(s,lines);
    }

    void collide_circles(sprite s1, sprite s2)
    {
        circle c1,c2;
        float col_normal_angle, a1, a2, opt_p, s1_mass, s2_mass;
        vector_2d n, s1_vel, s2_vel;

        s1_mass = sprite_mass(s1);
        s2_mass = sprite_mass(s2);
        s1_vel = sprite_velocity(s1);
        s2_vel = sprite_velocity(s2);

        if (s1_mass <= 0 || s2_mass <= 0)
        {
            LOG(WARNING) << "Collision with 0 or negative mass... ensure that mass is greater than 0";
            return;
        }

        c1 = sprite_collision_circle(s1);
        c2 = sprite_collision_circle(s2);

        //-- if s1^.mass < s2^.mass then
        // the sprite with larger velocity will be pushed out of collision, this probably should be changed to use mass
        if (vector_magnitude(s1_vel) > vector_magnitude(s2_vel))
        {
            //-- move s1 out
            n = vector_out_of_circle_from_circle(c1,c2, s1_vel);
            sprite_set_x(s1, sprite_x(s1) + n.x);
            sprite_set_y(s1, sprite_y(s1) + n.y);
        }
        else
        {
            //-- move s2 out
            n = vector_out_of_circle_from_circle(c2,c1, s2_vel);
            sprite_set_x(s2, sprite_x(s2) + n.x);
            sprite_set_y(s2, sprite_y(s2) + n.y);
        }

        // angle between sprites center point
        col_normal_angle = point_point_angle(center_point(s1), center_point(s2));

        //-- COLLISION RESPONSE
        //-- n = vector connecting the centers of the balls.
        //-- we are finding the components of the normalised vector n
        n = vector_to(cosine(col_normal_angle), sine(col_normal_angle));

        //-- now find the length of the components of each velocity vectors
        //-- along n, by using dot product.
        a1 = dot_product(s1_vel, n);
        a2 = dot_product(s2_vel, n);
        //LOG(DEBUG) << "VEL: " << a1 << "," << a2;

        //-- optimised P (momentum) = 2(a1 - a2) / (m1 + m2) 
        opt_p = (2.0 * (a1-a2)) / (s1_mass + s2_mass);
        //LOG(DEBUG) << "OPT_P: " << opt_p;

        //-- now find out the resultant vectors
        //-- Local r1% = c1.v - optimisedP * mass2 * n
        s1_vel.x -= (opt_p * s2_mass * n.x);
        s1_vel.y -= (opt_p * s2_mass * n.y);
        sprite_set_velocity(s1, s1_vel);

        //-- Local r2% = c2.v - optimisedP * mass1 * n
        s2_vel.x += (opt_p * s1_mass * n.x);
        s2_vel.y += (opt_p * s1_mass * n.y);
        sprite_set_velocity(s2, s2_vel);

    }

    // alternate sprite collision effects, has in/elastic coefficient
    void _collide_sprites_effects(sprite s1, sprite s2, const double &elas)
    {

        circle c1,c2;
        float col_normal_angle, a1, a2, p1, p2, inelas_mag, s1_mass, s2_mass, s1_mass_perc, s2_mass_perc;
        vector_2d n, s1_vel, s2_vel, rel_vel, out_vec;

        s1_mass = sprite_mass(s1);
        s2_mass = sprite_mass(s2);
        s1_vel = sprite_velocity(s1);
        s2_vel = sprite_velocity(s2);
        rel_vel = vector_subtract(s1_vel,s2_vel);  // relative velocity between s1 and s2, from s1 pov

        // center to center vector
        n = unit_vector(vector_from_to(s1,s2));

        c1 = sprite_collision_circle(s1);
        c2 = sprite_collision_circle(s2);
        out_vec = vector_out_of_circle_from_circle(c1, c2, rel_vel);    // calculate out of collision vector based on total velocity

        s1_mass_perc = s1_mass / (s1_mass + s2_mass);   // s1 fraction of total mass
        s2_mass_perc = 1 - s1_mass_perc;            // remainder value from 100%, could differ due to rounding
        //LOG(DEBUG) << "MASS: " << s1_mass_perc << "," << s2_mass_perc; 

        // move each sprite based on their inverse mass percentage
        move_sprite(s1, vector_multiply(out_vec, s2_mass_perc));
        move_sprite(s2, vector_multiply(out_vec, -s1_mass_perc));   // negative mult for opposite direction of vector

        // magnitude along center to center vector
        a1 = dot_product(s1_vel, n) / vector_magnitude(n);
        a2 = dot_product(s2_vel, n) / vector_magnitude(n);
        //LOG(DEBUG) << "VEL: " << a1 << "," << a2;
        
        // remove original velocity along vector
        sprite_add_to_velocity(s1, vector_multiply(n, -a1));
        sprite_add_to_velocity(s2, vector_multiply(n, -a2));
        
        // momentum along vector
        p1 = a1 * s1_mass;
        p2 = a2 * s2_mass;
        //LOG(DEBUG) << "P: " << p1 << ":" << p2;

        // elastic section, sprite momentum along vector is traded
        sprite_add_to_velocity(s1, vector_multiply(n, p2 / s1_mass * elas));    // takes s2 momentum
        sprite_add_to_velocity(s2, vector_multiply(n, p1 / s2_mass * elas));    // takes s1 momentum

        // inelastic section, momentum is combined
        inelas_mag = (p1 + p2) / (s1_mass + s2_mass) * (1 - elas);
        sprite_add_to_velocity(s1, vector_multiply(n, inelas_mag));
        sprite_add_to_velocity(s2, vector_multiply(n, inelas_mag));
        //LOG(DEBUG) << "ELAS: " << inelas_mag;

    }

    // test overload function behaviours
    void collide_circles(sprite s1, sprite s2, const double &elas)
    {

        float s1_mass, s2_mass;


        s1_mass = sprite_mass(s1);
        s2_mass = sprite_mass(s2);
        
        // if only one sprite is 0 mass, treat it as static, and use collide_circle_circle
        if (s1_mass <= 0 && s2_mass <= 0)
        {
            LOG(WARNING) << "Both collision sprites have 0 mass... ensure at least one is greater than 0";
            return;
        }

        if (s1_mass <= 0)
            collide_circle_circle(s2, sprite_collision_circle(s1));     // s1 no mass, receives no effect
        else if (s2_mass <= 0)
            collide_circle_circle(s1, sprite_collision_circle(s2));     // s2 no mass, receives no effect
        else
        {
            if (elas < 0 && elas > 1)
            {
                LOG(WARNING) << "Collision elasticity coefficient not between 0 to 1";
                return;
            }

            _collide_sprites_effects(s1, s2, elas);
        }
        
    }

    void collide_circle_circle(sprite s, const circle &c)
    {
        line hit_line;
        vector_2d out_vec, mvmt, normal, col_vec, hit_pt, centers_vector;   // hit_pt was originally a point_2d variable
        float mvmt_mag, prop;
        point_2d s_center;
        double col_mvmt_angle, overlap;
        

        if (s == NULL)
            return;

        //-- TODO: what if height > width!!
        s_center = center_point(s);
        mvmt = sprite_velocity(s);
        
        // re-consider this section, if the circle shape can initiate a collision on a static sprite
        //-- A not moving sprite cannot "collide" as it is stationary
        //-- We are then unabel to determine where the collision did
        //-- occur...
        if (vector_magnitude(mvmt) == 0)
        {
            LOG(WARNING) << "Attempting to perform sprite collision without sprite velocity";
            return;
        }

        centers_vector = vector_point_to_point(s_center,c.center);              // get vector from sprite to circle centers
        col_mvmt_angle = vector_angle_between(centers_vector, mvmt);
        //LOG(DEBUG) << col_mvmt_angle;
        
        //draw_line(COLOR_WHITE, line_from(s_center,vector_multiply(centers_vector,10)));
        //draw_line(COLOR_RED, line_from(s_center,vector_multiply(mvmt,10)));

        // if sprite velocity angle is perpendicular or facing away from collision, do not bounce
        if (col_mvmt_angle >= 90 || col_mvmt_angle <= -90)
        {
            // for circle, absolute distance between centers subtract position distance between objects
            overlap = (c.radius + sprite_collision_circle(s).radius) - vector_magnitude(centers_vector);
            //LOG(DEBUG) << overlap;

            // out vector along centers, magnitude based on distance under total radius. Rectangles need vector based on edge perp
            out_vec = vector_invert(vector_multiply(unit_vector(centers_vector), overlap));

            //draw_line(COLOR_WHITE, line_from(s_center,out_vec));
            move_sprite(s,out_vec);
        }
        else
        { 
            out_vec = vector_out_of_circle_from_circle(sprite_collision_circle(s), c, mvmt);
            //-- Back out of circle
            move_sprite(s, out_vec);

            //-- Normal of the collision...
            col_vec = unit_vector(vector_point_to_point(c.center, center_point(s)));
            normal = vector_normal(col_vec);
            
            hit_pt = vector_add(vector_to(c.center), vector_multiply(col_vec, float(c.radius + 1.42)));
            hit_line = line_from(point_offset_from_origin(vector_add(hit_pt, vector_multiply(normal, 100))), vector_multiply(normal, -100));
            
            //draw_sprite(s);
            //draw_circle(COLOR_YELLOW, hit_pt.x, hit_pt.y, 2);
            //draw_line(COLOR_YELLOW, hit_line);
    
            _collide_circle_line(s, hit_line);

            //-- do part velocity
            mvmt_mag = vector_magnitude(mvmt);
            prop = vector_magnitude(out_vec) / mvmt_mag;    //-- proportion of move "undone" by back out

            if (prop > 0)
                move_sprite(s, prop);       //-- TODO: Allow proportion of move to be passed in (overload)... then do velocity based on prop * pct
           
        }

        // refresh_screen(1);
        
    }

    //-- TODO: bounds based checking, need VectorIntoShape...
    void collide_circle_rectangle(sprite s, const rectangle &rect, bool bounds)
    {
        /*
        int hit_idx;
        vector<line> lines;
        vector_2d out_vec, mvmt;
        float mvmt_mag, prop;
        */

        if (s == NULL)
            return;

        collide_circle_lines(s, lines_from(rect));

        // ported code below needs fixing, substituted with another function above.

        /*
        mvmt = sprite_velocity(s);
        hit_idx = -1;

        //-- Get the line hit...
        lines = lines_from(rect);
        out_vec = vector_over_lines_from_circle(sprite_collision_circle(s), lines, mvmt, hit_idx);

        // variable name and conditional check differs from collied_circle_lines function
        if (hit_idx = -1)
            return;

        //-- back out of rectangle
        move_sprite(s, out_vec);

        //-- bounce...
        _collide_circle_line(s, lines[hit_idx]);

        //-- do part velocity
        mvmt_mag = vector_magnitude(mvmt);
        prop = vector_magnitude(out_vec) / mvmt_mag;    //-- proportion of move "undone" by back out

        if (prop > 0)
            move_sprite(s, prop);           //-- TODO: Allow proportion of move to be passed in (overload)... then do velocity based on prop * pct
        */
    }

    void collide_circle_rectangle(sprite s, const rectangle &rect)
    {
        collide_circle_rectangle(s, rect, false);
    }

    void collide_circle_triangle(sprite s, const triangle &tri)
    {
        collide_circle_lines(s, lines_from(tri));
    }

}