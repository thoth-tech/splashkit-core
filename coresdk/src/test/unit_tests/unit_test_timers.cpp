/**
 * Timer Unit Tests
 */

#include "catch.hpp"

#include "timers.h"

#include "logging_handling.h"

using namespace splashkit_lib;

TEST_CASE("test create and free timer", "[timers][free_timer][create_timer]")
{
    timer t = create_timer("test_timer");
    REQUIRE(has_timer("test_timer"));
    free_timer(t);
    REQUIRE_FALSE(has_timer("test_timer"));
}

TEST_CASE("pause", "[timers][free_timer][create_timer]")
{
    timer t2 = create_timer("test_pause");
    start_timer(t2);
    while (timer_ticks(t2) <= 0)
    {
    }
    REQUIRE(timer_ticks(t2) > 0);
    pause_timer(t2);
    unsigned int ticks = timer_ticks(t2);
    REQUIRE(timer_ticks(t2) == ticks);
}