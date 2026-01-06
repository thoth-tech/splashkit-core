/**
 * Timer Unit Tests
 */

#include "catch.hpp"

#include "timers.h"

#include "logging_handling.h"

using namespace splashkit_lib;

TEST_CASE("timmy timer", "[timers]")
{
    timer t = create_timer("test_timer");
    REQUIRE(has_timer("test_timer"));
    free_timer(t);
    REQUIRE_FALSE(has_timer("test_timer"));
}
