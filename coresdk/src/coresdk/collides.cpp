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

        // reverse the velocity towards closest point, needs check if it works
        s_vel.x += 2 * np_x;                                                        
        s_vel.y += 2 * np_y;
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
            n = vector_out_of_circle_from_circle(c1,c2, s2_vel);
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

        //-- optimised P (momentum) = 2(a1 - a2) / (m1 + m2)      //// i believe, this is an inelastic collision, where both sprites will combine their momentum
        opt_p = (2.0 * (a1-a2)) / (s1_mass + s2_mass);

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

    void collide_circle_circle(sprite s, const circle c)
    {
        line hit_line;
        vector_2d out_vec, mvmt, normal, col_vec, hit_pt;   // hit_pt was originally a point_2d variable
        float mvmt_mag, prop;
        point_2d s_center;
        

        if (s == NULL)
            return;

        //-- TODO: what if height > width!!
        s_center = center_point(s);
        mvmt = sprite_velocity(s);
        
        //-- A not moving sprite cannot "collide" as it is stationary
        //-- We are then unabel to determine where the collision did
        //-- occur...
        if (vector_magnitude(mvmt) == 0)
        {
            LOG(WARNING) << "Attempting to perform sprite collision without sprite velocity";
            return;
        }

        out_vec = vector_out_of_circle_from_circle(sprite_collision_circle(s), c, mvmt);
        //-- Back out of circle
        move_sprite(s, out_vec);

        //-- Normal of the collision...
        col_vec = unit_vector(vector_point_to_point(c.center, center_point(s)));
        normal = vector_normal(col_vec);
        
        // pascal source has some function overloads not existing now, NEEDS TO BE CHECKED IF WORKS CORRECTLY
        hit_pt = vector_add(vector_to(c.center), vector_multiply(col_vec, float(c.radius + 1.42)));
        hit_line = line_from(point_offset_from_origin(vector_add(hit_pt, vector_multiply(normal, 100))), point_offset_from_origin(vector_multiply(normal, -200)));

        //draw_sprite(s);
        //draw_line(hit_line);
        //refresh_screen(1);

        _collide_circle_line(s, hit_line);

        //-- do part velocity
        mvmt_mag = vector_magnitude(mvmt);
        prop = vector_magnitude(out_vec) / mvmt_mag;    //-- proportion of move "undone" by back out

        if (prop > 0)
            move_sprite(s, prop);       //-- TODO: Allow proportion of move to be passed in (overload)... then do velocity based on prop * pct
        
    }

    //-- TODO: bounds based checking, need VectorIntoShape...
    void collide_circle_rectangle(sprite s, const rectangle rect, bool bounds)
    {
        int hit_idx;
        vector<line> lines;
        vector_2d out_vec, mvmt;
        float mvmt_mag, prop;

        if (s == NULL)
            return;

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
    }

    void collide_circle_rectangle(sprite s, const rectangle rect)
    {
        collide_circle_rectangle(s, rect, false);
    }

    void collide_circle_triangle(sprite s, const triangle tri)
    {
        vector<line> lines;

        lines = lines_from(tri);

        collide_circle_lines(s, lines);
    }

}