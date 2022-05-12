/**
* Data Analytics Module Unit Tests
*
* Created by Timothy Moore and Ryan Lawrence
*
* Tests to be built to check desired end result, not to just make implemented code 
* within the data_analytics module pass tests. Fail cases must be included.
*/

#include "catch.hpp"

#include "data_analytics.h"

using namespace splashkit_lib;

TEST_CASE("data types can be validated")
{
    SECTION("can identify phone number")
    {
        REQUIRE(input_is_phone_number("String Test") == false);
        REQUIRE(input_is_phone_number("1") == false);
        REQUIRE(input_is_phone_number("0400123456") == true); // Mobile numbers in australia have 10 digits
        REQUIRE(input_is_phone_number("52001234") == true); //Landlines from Australia have 8 digits
        REQUIRE(input_is_phone_number("000123456789") == true); //Phone numbers from china have 12 digits
        
    }

    SECTION("can identify time entries")
    {
        REQUIRE(input_is_time("String Test") == false);
        REQUIRE(input_is_time("1") == false);
        REQUIRE(input_is_time("1:1") == false);
        REQUIRE(input_is_time("1:1:1") == false);
        REQUIRE(input_is_time("1:12") == false);
        REQUIRE(input_is_time("1:12:1") == false);
        REQUIRE(input_is_time("03:03:03") == true);
        REQUIRE(input_is_time("13:03:03") == true);
        REQUIRE(input_is_time("23:03:03") == true);
        REQUIRE(input_is_time("33:03:03") == false);
        REQUIRE(input_is_time("29:03:03") == false);
        REQUIRE(input_is_time("04:14:04") == true);
        REQUIRE(input_is_time("04:64:04") == false);
        REQUIRE(input_is_time("04:04:14") == true);
        REQUIRE(input_is_time("04:04:64") == false);
        REQUIRE(input_is_time("99:99:99") == false);
        REQUIRE(input_is_time("99:99:00") == false);
        REQUIRE(input_is_time("99:00:99") == false);
        REQUIRE(input_is_time("00:99:99") == false);
        REQUIRE(input_is_time("00:00:00") == true);
        REQUIRE(input_is_time("0:00:00") == true);
    }
}
