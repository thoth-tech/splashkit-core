/**
 * Timer Unit Tests
 */

#include "catch.hpp"
#include "timers.h"
#include "logging_handling.h"

using namespace splashkit_lib;

TEST_CASE("create_timer and free_timer", "[timers][create_timer][free_timer]")
{
    timer t = create_timer("test_timer");
    REQUIRE(has_timer("test_timer"));

    free_timer(t);
    REQUIRE_FALSE(has_timer("test_timer"));
}

TEST_CASE("start_timer and pause_timer", "[timers][start_timer][pause_timer]")
{
    timer t = create_timer("test_pause");
    REQUIRE(timer_ticks(t) == 0); // starts at 0

    start_timer(t);
    while (timer_ticks(t) == 0)
    {
        // wait until some ticks have passed
    }
    REQUIRE(timer_ticks(t) > 0); // time has advanced

    pause_timer(t);
    unsigned int ticks = timer_ticks(t);
    REQUIRE(timer_ticks(t) == ticks); // ticks do not change while paused
}
