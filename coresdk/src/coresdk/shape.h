#ifndef SHAPE_H
#define SHAPE_H

namespace splashkit_lib
{
    enum class shape_type
    {
        SPRITE,
        RECTANGLE,
        CIRCLE,
        TRIANGLE,
        QUAD,
    };
    
    struct rectangle;
    struct circle;
    struct triangle;
    struct quad;
    struct _sprite_data;
    struct vector_2d;

    struct shape
    {
        virtual ~shape() = default;
        virtual rectangle get_bounding_box() const = 0;
        virtual shape_type get_shape_type() const = 0;
        virtual bool intersects(const shape* other) const;
        virtual bool intersects(const shape& other) const;
        virtual bool intersects(const _sprite_data& other) const = 0;
        virtual bool intersects(const rectangle& other) const = 0;
        virtual bool intersects(const circle& other) const = 0;
        virtual bool intersects(const triangle& other) const = 0;
        virtual bool intersects(const quad& other) const = 0;
        virtual bool AABB_intersects(const shape* other) const;
        virtual void move_by(const vector_2d& amount);
    };
}

#endif /* SHAPE_H */