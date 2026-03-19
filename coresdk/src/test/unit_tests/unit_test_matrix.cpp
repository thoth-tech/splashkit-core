/**
 * Matrix Unit Tests
 */


#include "catch.hpp"
#include "physics.h"

using namespace splashkit_lib;

TEST_CASE("identity_matrix returns the identity matrix", "[identity_matrix]")
{
    matrix_2d matrix = identity_matrix();

    REQUIRE(matrix_to_string(matrix) == matrix_to_string(identity_matrix()));
}

TEST_CASE("matrix_inverse of identity_matrix returns the identity matrix", "[matrix_inverse][identity_matrix]")
{
    matrix_2d matrix = identity_matrix();
    matrix_2d inv_matrix = matrix_inverse(matrix);

    REQUIRE(matrix_to_string(inv_matrix) == matrix_to_string(identity_matrix()));
}

TEST_CASE("scale_matrix multiplied by its inverse returns the identity matrix", "[scale_matrix][matrix_inverse][matrix_multiply]")
{
    matrix_2d matrix = scale_matrix(4);
    matrix_2d inv_matrix = matrix_inverse(matrix);

    REQUIRE(matrix_to_string(matrix_multiply(matrix, inv_matrix)) == matrix_to_string(identity_matrix()));
}

TEST_CASE("rotation_matrix multiplied by its inverse returns the identity matrix", "[rotation_matrix][matrix_inverse][matrix_multiply]")
{
    matrix_2d matrix = rotation_matrix(45);
    matrix_2d inv_matrix = matrix_inverse(matrix);

    REQUIRE(matrix_to_string(matrix_multiply(matrix, inv_matrix)) == matrix_to_string(identity_matrix()));
}

TEST_CASE("translation_matrix multiplied by its inverse returns the identity matrix", "[translation_matrix][matrix_inverse][matrix_multiply]")
{
    matrix_2d matrix = translation_matrix(100, -100);
    matrix_2d inv_matrix = matrix_inverse(matrix);

    REQUIRE(matrix_to_string(matrix_multiply(matrix, inv_matrix)) == matrix_to_string(identity_matrix()));
}

TEST_CASE("matrix_multiply with identity_matrix returns the original matrix", "[matrix_multiply][identity_matrix]")
{
    matrix_2d matrix = rotation_matrix(45);

    REQUIRE(matrix_to_string(matrix_multiply(matrix, identity_matrix())) == matrix_to_string(matrix));
    REQUIRE(matrix_to_string(matrix_multiply(identity_matrix(), matrix)) == matrix_to_string(matrix));
}

TEST_CASE("scale_matrix with factor 1 matches the identity matrix", "[scale_matrix]")
{
    matrix_2d matrix = scale_matrix(1);

    REQUIRE(matrix_to_string(matrix) == matrix_to_string(identity_matrix()));
}

TEST_CASE("translation_matrix with zero offset matches the identity matrix", "[translation_matrix]")
{
    matrix_2d matrix = translation_matrix(0, 0);

    REQUIRE(matrix_to_string(matrix) == matrix_to_string(identity_matrix()));
}

TEST_CASE("combination matrix multiplied by its inverse returns the identity matrix", "[matrix_multiply][matrix_inverse][rotation_matrix][translation_matrix]")
{
    matrix_2d matrix = matrix_multiply(rotation_matrix(45), translation_matrix(10, 10));
    matrix_2d inv_matrix = matrix_inverse(matrix);

    REQUIRE(matrix_to_string(matrix_multiply(matrix, inv_matrix)) == matrix_to_string(identity_matrix()));
}

TEST_CASE("matrix_to_string formats the identity matrix correctly", "[matrix_to_string][identity_matrix]")
{
    matrix_2d matrix = identity_matrix();

    string expected =
        " ------------------------------\n"
        "|        1         0         0 |\n"
        "|        0         1         0 |\n"
        "|        0         0         1 |\n"
        " ------------------------------";

    REQUIRE(matrix_to_string(matrix) == expected);
}

TEST_CASE("matrix_to_string formats a translation matrix correctly", "[matrix_to_string][translation_matrix]")
{
    matrix_2d matrix = translation_matrix(10, 10);

    string expected =
        " ------------------------------\n"
        "|        1         0        10 |\n"
        "|        0         1        10 |\n"
        "|        0         0         1 |\n"
        " ------------------------------";

    REQUIRE(matrix_to_string(matrix) == expected);
}