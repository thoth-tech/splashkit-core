/**
 * @header  collides
 * @author  RO
 * @brief   
 *
 * @attribute group  physics
 * @attribute static collides
 */

#ifndef collides_h
#define collides_h

//#include "images.h"
#include "matrix_2d.h"
#include "sprites.h"
#include "types.h"
namespace splashkit_lib
{

    /**
     * Sprite circle to line collision effect
     * Moves sprite out of collision of line
     * Bounces sprite velocity perpendicular to line
     *
     * @param s             The sprite to test
     * @param line_segment  The line to collide with      
     *
     * @attribute class collide
     * @attribute method circle_line
     *
     * 
     */
    void collide_circle_line(sprite s, const line &line_segment);

    /**
     * Sprite circle to circle collision effect
     * Moves sprite out of collision with circle
     * Bounces from center points
     *
     * @param s             The sprite to test
     * @param c             The circle to collide with
     *
     * @attribute class collide
     * @attribute method circle_circle
     *
     * 
     */  
    void collide_circle_circle(sprite s, const circle& c);

    /**
     * Sprite circle to rectangle collision effect
     * Moves sprite out of collision with rectangle
     * Bounces based on which sides in collision
     *
     * @param s             The sprite to collide with
     * @param rect          The rectangle to collide with
     *
     * @attribute class collide
     * @attribute method circle_rectangle
     *
     * 
     */
    void collide_circle_rectangle(sprite s, const rectangle& rect);

    /**
     * Sprite circle to triangle collision effect
     * Moves out of collision from its lines
     * Bounces based on which line is closest to collision
     *
     * @param s             The sprite to collide with
     * @param tri           The triangle to collide with
     *
     * @attribute class collide
     * @attribute method circle_triangle
     *
     * 
     */
    void collide_circle_triangle(sprite s, const triangle& tri);

    /**
     * Sprite circle to sprite circle collision effect
     * Moves faster sprite out of collision
     * Sprites need positive mass values
     * 
     *
     * @param s1            The first sprite to collide with
     * @param s2            The second sprite to collide with
     *
     * @attribute class collide
     * @attribute method circles
     *
     * 
     */
    void collide_circles(sprite s1, sprite s2);

    /**
     * Sprite circle to sprite circle collision effect
     * test function behaviour
     * needs more testing, debugging to verify effects
     *
     * @param s1            The first sprite to collide with
     * @param s2            The second sprite to collide with
     * @param elas          0-1 value coefficient for elastic to inelastic result
     *
     * @attribute class collide
     * @attribute method circles
     *
     * 
     */
    void collide_circles(sprite s1, sprite s2, const double &elas);

}
#endif /* collides_h*/