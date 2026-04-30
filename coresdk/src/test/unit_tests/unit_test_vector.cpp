/**
 * Vector Unit Tests
 */

#include "catch.hpp"

#include "types.h"
#include "vector_2d.h"

using namespace splashkit_lib;

TEST_CASE("can correctly create vectors", "[vector]")
{
    SECTION("can create a vector from x and y values")
    {
        vector_2d v = vector_to(3.0, 4.0);

        REQUIRE(v.x == 3.0);
        REQUIRE(v.y == 4.0);
    }

    SECTION("can create a zero vector")
    {
        vector_2d v = vector_to(0.0, 0.0);

        REQUIRE(v.x == 0.0);
        REQUIRE(v.y == 0.0);
    }
}

TEST_CASE("can correctly perform basic vector operations", "[vector]")
{
    SECTION("can add two vectors")
    {
        vector_2d result = vector_add(
            vector_to(3.0, 4.0),
            vector_to(2.0, 6.0)
        );

        REQUIRE(result.x == 5.0);
        REQUIRE(result.y == 10.0);
    }

    SECTION("can subtract two vectors")
    {
        vector_2d result = vector_subtract(
            vector_to(7.0, 9.0),
            vector_to(2.0, 4.0)
        );

        REQUIRE(result.x == 5.0);
        REQUIRE(result.y == 5.0);
    }

    SECTION("can multiply vector by scalar")
    {
        vector_2d result = vector_multiply(
            vector_to(3.0, 4.0),
            2.0
        );

        REQUIRE(result.x == 6.0);
        REQUIRE(result.y == 8.0);
    }

    SECTION("can calculate dot product")
    {
        double result = dot_product(
            vector_to(3.0, 4.0),
            vector_to(2.0, 5.0)
        );

        REQUIRE(result == 26.0);
    }

    SECTION("can detect equal vectors")
    {
        REQUIRE(
            vectors_equal(
                vector_to(3.0, 4.0),
                vector_to(3.0, 4.0)
            )
        );
    }

    SECTION("can detect different vectors")
    {
        REQUIRE_FALSE(
            vectors_equal(
                vector_to(3.0, 4.0),
                vector_to(1.0, 2.0)
            )
        );
    }
}
TEST_CASE("can correctly calculate vector properties", "[vector]")
{
    SECTION("can calculate vector magnitude")
    {
        vector_2d v = vector_to(3.0, 4.0);

        REQUIRE(vector_magnitude(v) == 5.0);
    }

    SECTION("can calculate vector magnitude squared")
    {
        vector_2d v = vector_to(3.0, 4.0);

        REQUIRE(vector_magnitude_squared(v) == 25.0);
    }

    SECTION("can create unit vector")
    {
        vector_2d result = unit_vector(vector_to(3.0, 4.0));

        REQUIRE(result.x == Approx(0.6));
        REQUIRE(result.y == Approx(0.8));
    }

    SECTION("can detect zero vector")
    {
        REQUIRE(is_zero_vector(vector_to(0.0, 0.0)));
        REQUIRE_FALSE(is_zero_vector(vector_to(1.0, 0.0)));
    }

    SECTION("can convert vector to string")
    {
        vector_2d v = vector_to(3.0, 4.0);

        REQUIRE(vector_to_string(v) == "Vec -> 3.000000:4.000000");
    }
}