//
//  test_circle_collide.cpp
//  splashkit
//
//  Created by RO on 05/09/2023.
//  Copyright © 2023 RO. All rights reserved.
//
//  -- marks pascal source comments

#include "collides.h"
#include "collisions.h"
#include "graphics.h"
#include "input.h"
#include "sprites.h"
#include "window_manager.h"
#include "audio.h"
#include "utility_functions.h"

#include <cmath>

using namespace splashkit_lib;
using std::function;

void keep_on_screen(sprite s)
{
    if (sprite_x(s) > screen_width() - sprite_width(s))
    {
        sprite_set_dx(s, -sprite_dx(s));
        sprite_set_x(s, screen_width() - sprite_width(s));
    }

    if (sprite_y(s) > screen_height() - sprite_height(s))
    {
        sprite_set_dy(s, -sprite_dy(s));
        sprite_set_y(s, screen_height() - sprite_height(s));
    }

    if (sprite_x(s) < 0)
    {
        sprite_set_dx(s, -sprite_dx(s));
        sprite_set_x(s, 0);
    }

    if (sprite_y(s) < 0)
    {
        sprite_set_dy(s, -sprite_dy(s));
        sprite_set_y(s, 0);
    }
    
}

struct double_pt
{
    point_2d pt_on_circle;
    point_2d pt_on_line;
};

void do_line_test(vector<line> test_lines, const point_2d &center, long radius, vector_2d &movement)
{
    point_2d pt1, pt2, pt_on_line, pt_on_circle, hit_pt, max_hit_pt_on_line, max_hit_pt_on_circle, out_pt;
    point_2d tmp[4];
    double_pt chk_pts[4];
    vector_2d line_vec, normal_mvmt, normal_line, to_edge, edge, ray, v_out;
    int max_idx, hits;
    float dot_prod, dist, max_dist;

    //-- Cast ray searching for points back from shape
    ray = movement;     // removed vector invert, post out of collision circle is behind velocity now, instead of opposite side of line
    normal_mvmt = vector_normal(movement);

    max_idx = -1;
    max_dist = -1;

    //-- Search all lines for hit points
    for (int i = 0; i < test_lines.size(); i++)
    {
        line_vec = vector_from_line(test_lines[i]);
        //-- Get the normal of the line we hit
        normal_line = vector_normal(line_vec);
        hits = 0;

        //-- tmp 0 and tmp 1 are the widest points, to detect collision with line
        widest_points(circle_at(center, radius), normal_mvmt, tmp[0], tmp[1]);
        //-- tmp 2 and tmp 3 are the closest and furthest points from the line
        widest_points(circle_at(center, radius), normal_line, tmp[2], tmp[3]);

        for (int j = 0; j < 3; j++)
        {
            // draw_circle(COLOR_WHITE, tmp[j], 2);

            //-- Casta ray back from the test points to find line pts... out on ptOnLine
            if (ray_intersection_point(tmp[j], ray, test_lines[i], pt_on_line))
            {
                // draw_circle(COLOR_RED, pt_on_line, 1);
                // draw_line(COLOR_RED, tmp[j], pt_on_line);

                chk_pts[hits].pt_on_line = pt_on_line;
                chk_pts[hits].pt_on_circle = tmp[j];
                hits++;
            }
        }

        //-- for each of the hits on this line...
        //-- search for the longest hit.
        for (int j = 0; j < hits - 1; j++)
        {
            // draw_circle(COLOR_WHITE, chk_pts[j].pt_on_circle, 1);
            
            to_edge = vector_point_to_point(center, chk_pts[j].pt_on_circle);
            
            // draw_line(COLOR_RED, center, chk_pts[j].pt_on_circle);

            dot_prod = dot_product(to_edge, normal_line);

            //-- 2d: Most distant edge pt is on a line this distance (dotProd) from the center
            edge = vector_add(vector_to(center), vector_multiply(normal_line,dot_prod));
            // draw_pixel(COLOR_WHITE,edge); //-- Draws pt on line to distant pt

            //-- Find the point we hit on the line... ptOnLine receives intersection point...
            if (not line_intersection_point(line_from(point_offset_from_origin(edge), movement), test_lines[i], pt_on_line)) continue;
            // Move back onto line segment... linePt -> closest point on line to intersect point
            // draw_circle(COLOR_RED, pt_on_line, 1);
            // draw_line(COLOR_WHITE, edge, pt_on_line);

            pt_on_line = closest_point_on_line(pt_on_line, test_lines[i]);
            // fill_circle(COLOR_BLUE, pt_on_line, 1);
            
            //-- Find the most distant point on the circle, given the movement vector
            if (not distant_point_on_circle_heading(pt_on_line, circle_at(center,radius), movement, pt_on_circle)) continue;
            // fill_circle(COLOR_BLUE, pt_on_circle, 2); //-- point on segment
            // draw_line(COLOR_BLUE, pt_on_line, pt_on_circle);

            dist = point_point_distance(pt_on_line, pt_on_circle);
            // LOG(DEBUG) << dist;
            if (dist > max_dist || max_idx == -1)
            {
                max_dist = dist;
                max_idx = i;
                v_out = vector_point_to_point(pt_on_circle, pt_on_line);
                // dot_prod = dot_product(unit_vector(v_out), vector_to(0.5,0.5));
                v_out = vector_multiply(unit_vector(v_out), vector_magnitude(v_out) + 1.42);
                // LOG(DEBUG) << dot_prod << 4 << 2    // not precisely sure of the pascal source detail
            }
        }
    }

    if (max_idx > -1)
    {
        // LOG(DEBUG) << "---";
        draw_line(COLOR_GREEN, line_from(center, v_out));
        out_pt = point_offset_from_origin(vector_add(vector_to(center), v_out));
        draw_circle(COLOR_GREEN, out_pt.x, out_pt.y, radius);
    }
    
}

void check_collision_with_line(sprite s, const line &line_segment)
{
    if (line_intersects_circle(line_segment, sprite_collision_circle(s)))
    {
        collide_circle_line(s, line_segment);
    }
}

void check_collision_with_triangle(sprite s, const triangle &t)
{
    if (circle_triangle_collision(sprite_collision_circle(s), t))
    {
        collide_circle_triangle(s, t);
    }
}

// function from vector_2d.cpp, needs substitution, suggest a vector out circle from triangle
//vector_2d vector_over_lines_from_circle(const circle &c, const vector<line> lines, const vector_2d &velocity, int &max_idx);

void run_circle_collide_test()
{
    point_2d c1, rect_pt, pt_out, temp, tmp, edge;
    long r1, r2;
    rectangle rect, rect1;
    sprite s1, s2;
    line found;
    vector_2d mouse_mvmt, mouse_out, pt_on_circle, to_center, opposite_pt, to_circle;
    matrix_2d nrm, rm;
    triangle t1;

    int max_idx;
    float dist, dot_prod;
    point_2d check_points[3];
    vector<line> test_lines = vector<line>(1);

    open_audio();
    open_window("Circle Collisions", 800, 600);
    //LoadDefaultColors();          //not sure what this does from swingame, seems unused in SK
    load_bitmap("ball","ball_small.png");

    test_lines[0] = line_from(300,240,500,350);

    rm = rotation_matrix(1.0);
    nrm = rotation_matrix(-1.0);

    c1 = point_at(100,100);
    r1 = 50;
    r2 = 20;

    // Setup S1
    s1 = create_sprite("ball");

    sprite_set_x(s1, 100);
    sprite_set_y(s1, 600);
    sprite_set_mass(s1, 10);
    sprite_set_velocity(s1, vector_to(4,4));

    // Setup S2
    s2 = create_sprite("ball");

    sprite_set_x(s2, 200);
    sprite_set_y(s2, 200);
    sprite_set_mass(s2, 10);
    sprite_set_velocity(s2, vector_to(1,-5));

    t1 = triangle_from(600,100,550,200,670,175);

    mouse_mvmt = vector_to(1,1);

    rect = rectangle_from(400,300,200,200);
    rect1 = rectangle_from(420,420,10,20);

    show_mouse(false);

    while ( not quit_requested() )  //-- The game loop...
    {
        process_events();

        clear_screen(COLOR_BLACK);

        draw_rectangle(COLOR_RED, rect);
        //draw_rectangle(COLOR_RED, rect1);     // rect1 is smaller rectangle inside rect
        draw_circle(COLOR_RED, c1.x, c1.y, r1);
        draw_triangle(COLOR_RED, t1);

        temp = mouse_position();

        for (int i = 0; i < test_lines.size(); i++)
        {
            if (line_intersects_circle(test_lines[i],circle_at(temp, r2)))
                draw_line(COLOR_WHITE, test_lines[i]);
            else
                draw_line(COLOR_RED, test_lines[i]);
        }

        

        
        do_line_test(test_lines, temp, r2, mouse_mvmt);

        


        if (circle_rectangle_collision(circle_at(temp, r2), rect))  // check mouse circle against test rectangle
        {
            draw_circle(COLOR_BLUE, temp.x, temp.y, r2);
            draw_rectangle(COLOR_WHITE, rect);

            // draw mouse circle out of collision, based on velocity
            mouse_out = vector_out_of_rect_from_circle(circle_at(temp, r2), rect, mouse_mvmt);
            draw_circle(COLOR_GREEN, temp.x + mouse_out.x, temp.y + mouse_out.y, r2);
        }
        else if (circles_intersect(circle_at(temp, r2), circle_at(c1,r1)))  // check mouse circle against test circle
        {
            draw_circle(COLOR_BLUE, temp.x, temp.y, r2);
            draw_circle(COLOR_WHITE, c1.x, c1.y, r1);

            // draw mouse circle out of collision, based on velocity
            mouse_out = vector_out_of_circle_from_circle(circle_at(temp, r2), circle_at(c1, r1), mouse_mvmt);
            draw_circle(COLOR_GREEN, temp.x + mouse_out.x, temp.y + mouse_out.y, r2);
        }
        else if (circle_triangle_collision(circle_at(temp, r2), t1))        // check mouse circle against test triangle
        {
            draw_circle(COLOR_BLUE, temp.x, temp.y, r2);
            draw_triangle(COLOR_WHITE, t1);
            
            mouse_out = vector_out_of_triangle_from_circle(circle_at(temp, r2), t1, mouse_mvmt);
            draw_circle(COLOR_GREEN, temp.x + mouse_out.x, temp.y + mouse_out.y, r2);

            draw_line(COLOR_WHITE, lines_from(t1)[max_idx]);
            

            //-- LineOfLinesCircleHit, Function not in splashkit
        }
        else
        {
            draw_circle(COLOR_GREEN, temp.x, temp.y, r2);
        }

        // rect_pt = closest_point_on_rect_from_circle(temp, r2, rect);
        // pt_out = DeepestPointOnCircleVsRectWithMovement                          // Function not in splashkit
        // mouse_out = vector_out_of_rect_from_circle(temp, r2, rect, mouse_mvmt);

        // rotate mouse circle supposed velocity right/left
        if (key_down(RIGHT_KEY))
            mouse_mvmt = matrix_multiply(rm, mouse_mvmt);
        if (key_down(LEFT_KEY))
            mouse_mvmt = matrix_multiply(nrm, mouse_mvmt);

        // move mouse circle along supposed velocity, back or forward
        if (key_down(R_KEY))
        {
            tmp = point_offset_from_origin(vector_add(vector_invert(mouse_mvmt), vector_to(temp)));
            move_mouse(round(tmp.x), round(tmp.y));
        }
        else if (key_down(F_KEY))
        {
            tmp = point_offset_from_origin(vector_add(mouse_mvmt, vector_to(temp)));
            move_mouse(round(tmp.x), round(tmp.y));
        }

        if (key_down(UP_KEY))
            r2 += 2;
        if (key_down(DOWN_KEY))
            r2 -= 2;

        // draw_circle(COLOR_YELLOW, pt_out, 2);
        // draw_line(COLOR_WHITE, temp, rect_pt);
        // draw_line(COLOR_YELLOW, temp, pt_out);

        draw_line(COLOR_BLUE, temp.x, temp.y, temp.x + (mouse_mvmt.x * 10), temp.y + (mouse_mvmt.y * 10));
        // draw_line(COLOR_YELLOW, temp.x, temp.y, temp.x + mouse_out.x, temp.y + mouse_out.y);

        // temp = point_at(temp, mouse_out);
        // draw_circle(COLOR_YELLOW, temp, r2);

        draw_sprite(s1);
        temp = center_point(s1);
        draw_circle(COLOR_BLUE, sprite_collision_circle(s1));
        draw_line(COLOR_BLUE, temp.x, temp.y, temp.x + (sprite_dx(s1) * 10), temp.y + (sprite_dy(s1) * 10));
        update_sprite(s1);
        keep_on_screen(s1);

        if (circle_rectangle_collision(sprite_collision_circle(s1), rect))
            collide_circle_rectangle(s1, rect);
        else if (circles_intersect(sprite_collision_circle(s1), circle_at(c1, r1)))
            collide_circle_circle(s1, circle_at(c1,r1));

        draw_sprite(s2);
        temp = center_point(s2);
        draw_circle(COLOR_YELLOW, sprite_collision_circle(s2));
        draw_line(COLOR_YELLOW, temp.x, temp.y, temp.x + (sprite_dx(s2) * 10), temp.y + (sprite_dy(s2) * 10));
        update_sprite(s2);
        keep_on_screen(s2);

        if (circle_rectangle_collision(sprite_collision_circle(s2), rect))
            collide_circle_rectangle(s2, rect);
        else if (circles_intersect(sprite_collision_circle(s2), circle_at(c1, r1)))
            collide_circle_circle(s2, circle_at(c1,r1));

        check_collision_with_line(s1, test_lines[0]);
        check_collision_with_line(s2, test_lines[0]);

        check_collision_with_triangle(s1, t1);
        check_collision_with_triangle(s2, t1);

        temp = mouse_position();
        if (circles_intersect(sprite_collision_circle(s1), circle_at(temp, r2)))
            collide_circle_circle(s1, circle_at(temp, r2));
        if (circles_intersect(sprite_collision_circle(s2), circle_at(temp, r2)))
            collide_circle_circle(s2, circle_at(temp, r2));

        if (circles_intersect(sprite_collision_circle(s1), sprite_collision_circle(s2)))
            collide_circles(s1,s2);

        // yellow line drawn to show vector of mouse position to be moved out of collision
        //temp = point_offset_from_origin(vector_out_of_rect_from_point(temp, rect, mouse_mvmt));
        //draw_line(COLOR_YELLOW, mouse_position(), point_offset_from_origin(vector_add(vector_to(temp), vector_to(mouse_position()))));

        // draw line for rect1 for its out of collision vector on rect
        //temp = point_offset_from_origin(vector_out_of_rect_from_rect(rect1, rect, mouse_mvmt));
        //draw_line(COLOR_YELLOW, point_at(rect1.x, rect1.y), point_offset_from_origin(vector_add(vector_to(temp), vector_to(rect1.x, rect1.y))));

        // temp = VectorIntoRectFromRect(rect1,rect,mouse_mvmt);    //function not found in splashkit
        // draw_line(COLOR_WHITE, vector_to(rect1.x, rect1.y), vector_add(temp, vector_to(rect1.x, rect1.y)));

        //-- DrawFramerate(0,0);    //function not found in splashkit
        refresh_screen(60);

    } 

    close_audio();
    close_window("Circle Collisions");
    
}