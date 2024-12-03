//
//  types.cpp
//  splashkit
//
//  Created by Andrew Cain on 24/07/2016.
//  Copyright © 2016 Andrew Cain. All rights reserved.
//

#include "types.h"
#include "collisions.h"
#include "sprites.h"

namespace splashkit_lib
{
    bool shape::intersects(const shape* other) const
    {
        switch (other->get_shape_type())
        {
            case shape_type::SPRITE:
                return other->intersects(*this);
            case shape_type::RECTANGLE:
                return this->intersects(*dynamic_cast<const rectangle*>(other));
            case shape_type::CIRCLE:
                return this->intersects(*dynamic_cast<const circle*>(other));
            case shape_type::TRIANGLE:
                return this->intersects(*dynamic_cast<const triangle*>(other));
            case shape_type::QUAD:
                return this->intersects(*dynamic_cast<const quad*>(other));
        };

        return false;
    }

    bool shape::intersects(const shape& other) const
    {
        return this->intersects(&other);
    }

    bool shape::AABB_intersects(const shape* other) const
    {
        return this->get_bounding_box().intersects(other->get_bounding_box());
    }
    
    rectangle::rectangle()
    {
        x = 0.0;
        y = 0.0;
        width = 0.0;
        height = 0.0;
    }
    
    rectangle::rectangle(double x, double y, double width, double height)
    {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }
    
    rectangle rectangle::get_bounding_box() const
    {
        return *this;
    }

    shape_type rectangle::get_shape_type() const
    {
        return shape_type::RECTANGLE;
    }

    bool rectangle::intersects(const rectangle& other) const
    {
        return rectangles_intersect(*this, other);
    }

    bool rectangle::intersects(const circle& other) const
    {
        return rectangle_circle_intersect(*this, other);
    }

    bool rectangle::intersects(const triangle& other) const
    {
        return triangle_rectangle_intersect(other, *this);
    }

    bool rectangle::intersects(const quad& other) const
    {
        return quads_intersect(quad_from(*this), other);
    }

    quad::quad()
    {
        for (int i = 0; i < 4; i++)
        {
            points[i] = {0.0, 0.0};
        }
    }

    quad::quad(const point_2d& top_left, const point_2d& top_right,
            const point_2d& bottom_left, const point_2d& bottom_right)
    {
        points[0] = top_left;
        points[1] = top_right;
        points[2] = bottom_left;
        points[3] = bottom_right;
    }

    rectangle quad::get_bounding_box() const
    {
        return rectangle_around(*this);
    }

    shape_type quad::get_shape_type() const
    {
        return shape_type::QUAD;
    }

    bool quad::intersects(const rectangle& other) const
    {
        return other.intersects(*this);
    }

    bool quad::intersects(const circle& other) const
    {
        return other.intersects(*this);
    }

    bool quad::intersects(const triangle& other) const
    {
        return other.intersects(*this);
    }

    bool quad::intersects(const quad& other) const
    {
        return other.intersects(*this);
    }

    circle::circle()
    {
        center = {0.0, 0.0};
        radius = 0.0;
    }

    circle::circle(const point_2d& center, double radius)
    {
        this->center = center;
        this->radius = radius;
    }

    rectangle circle::get_bounding_box() const
    {
        return rectangle_around(*this);
    }

    shape_type circle::get_shape_type() const
    {
        return shape_type::CIRCLE;
    }

    bool circle::intersects(const rectangle& other) const
    {
        return other.intersects(*this);
    }

    bool circle::intersects(const circle& other) const
    {
        return circles_intersect(*this, other);
    }

    bool circle::intersects(const triangle& other) const
    {
        return circle_triangle_intersect(*this, other);
    }

    bool circle::intersects(const quad& other) const
    {
        return circle_quad_intersect(*this, other);
    }

    triangle::triangle()
    {
        for (int i = 0; i < 3; i++)
        {
            points[i] = {0.0, 0.0};
        }
    }

    triangle::triangle(const point_2d& p1, const point_2d& p2, const point_2d& p3)
    {
        points[0] = p1;
        points[1] = p2;
        points[2] = p3;
    }

    rectangle triangle::get_bounding_box() const
    {
        return rectangle_around(*this);
    }

    shape_type triangle::get_shape_type() const
    {
        return shape_type::TRIANGLE;
    }

    bool triangle::intersects(const rectangle& other) const
    {
        return other.intersects(*this);
    }

    bool triangle::intersects(const circle& other) const
    {
        return other.intersects(*this);
    }

    bool triangle::intersects(const triangle& other) const
    {
        return triangles_intersect(*this, other);
    }

    bool triangle::intersects(const quad& other) const
    {
        return triangle_quad_intersect(*this, other);
    }
}
