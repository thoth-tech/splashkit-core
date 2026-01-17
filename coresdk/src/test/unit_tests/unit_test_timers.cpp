/**
 * Timer Unit Tests
 */

#include "catch.hpp"
#include "timers.h"
#include "logging_handling.h"
#include "utils.h"

using namespace splashkit_lib;

TEST_CASE(
    "timer can be created",
    "[timers][create_timer]")
{
    timer t = create_timer("test");
    REQUIRE(has_timer("test"));
    free_timer(t);
}

TEST_CASE(
    "timer starts at zero ticks",
    "[timers][create_timer][timer_ticks]")
{
    timer t = create_timer("test");
    REQUIRE(timer_ticks(t) == 0);
    free_timer(t);
}

TEST_CASE(
    "timer advances after start",
    "[timers][start_timer][timer_ticks]")
{
    timer t = create_timer("test");
    start_timer(t);

    delay(200);

    REQUIRE(timer_ticks(t) > 0);

    free_timer(t);
}

TEST_CASE(
    "timer does not advance while paused",
    "[timers][pause_timer][timer_ticks]")
{
    timer t = create_timer("test");
    start_timer(t);
    delay(200);

    pause_timer(t);
    unsigned int ticks = timer_ticks(t);
    delay(200);

    REQUIRE(timer_ticks(t) == ticks);

    free_timer(t);
}

TEST_CASE(
    "timer can be freed",
    "[timers][free_timer]")
{
    timer t = create_timer("test");
    free_timer(t);

    REQUIRE_FALSE(has_timer("test"));
}
