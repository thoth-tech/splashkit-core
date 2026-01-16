/**
 * Timer Unit Tests
 */

#include "catch.hpp"
#include "timers.h"
#include "logging_handling.h"

using namespace splashkit_lib;

TEST_CASE(
    "timer can be created, started, paused, and freed",
    "[timers][create_timer][free_timer][start_timer][pause_timer]")
{
    timer t = create_timer("test");
    REQUIRE(has_timer("test"));

    REQUIRE(timer_ticks(t) == 0); // starts at 0

    int attempts = 0;
    start_timer(t);
    while (timer_ticks(t) == 0 || attempts < 1000)
    {
        attempts++;
    };

    REQUIRE(timer_ticks(t) > 0); // time has advanced

    pause_timer(t);
    unsigned int ticks = timer_ticks(t);
    REQUIRE(timer_ticks(t) == ticks); // ticks do not change while paused

    free_timer(t);
    REQUIRE_FALSE(has_timer("test")); // check the same name we created
}
