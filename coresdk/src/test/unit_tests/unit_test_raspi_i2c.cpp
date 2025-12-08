/**
 * Raspi I2C Unit Tests
 *
 * Tests for the I2C device management API.
 * Note: Hardware-dependent tests (actual I2C communication) require a Raspberry Pi.
 * These tests focus on the device registry and error handling that works on all platforms.
 */

#include "catch.hpp"

#include "types.h"
#include "raspi_i2c.h"
#include "logging_handling.h"

using namespace splashkit_lib;

TEST_CASE("I2C device registry functions work correctly", "[i2c][raspi]")
{
    // Clean up any devices from previous tests
    close_all_i2c_devices();

    SECTION("has_i2c_device returns false for non-existent device")
    {
        REQUIRE_FALSE(has_i2c_device("non_existent_device"));
    }

    SECTION("i2c_device_named returns nullptr for non-existent device")
    {
        disable_logging(WARNING); // Disable "I2C device not loaded" warning
        i2c_device dev = i2c_device_named("non_existent_device");
        enable_logging(WARNING);
        
        REQUIRE(dev == nullptr);
    }

    SECTION("close_i2c_device handles null pointer gracefully")
    {
        disable_logging(WARNING); // Disable warning about null device
        close_i2c_device(nullptr);
        enable_logging(WARNING);
        
        // Should not crash
        REQUIRE(true);
    }

    SECTION("close_i2c_device handles non-existent name gracefully")
    {
        disable_logging(WARNING); // Disable warning about non-existent device
        close_i2c_device("non_existent_device");
        enable_logging(WARNING);
        
        // Should not crash
        REQUIRE(true);
    }

    SECTION("close_all_i2c_devices handles empty registry gracefully")
    {
        close_all_i2c_devices();
        
        // Should not crash and registry should still be empty
        REQUIRE_FALSE(has_i2c_device("any_device"));
    }
}

TEST_CASE("I2C device type enum has expected values", "[i2c][types]")
{
    SECTION("I2C device types have correct values")
    {
        REQUIRE(I2C_DEVICE_GENERIC == 0);
        REQUIRE(I2C_DEVICE_HT16K33_8X8 == 1);
        REQUIRE(I2C_DEVICE_HT16K33_14SEG == 2);
        REQUIRE(I2C_DEVICE_PCF8574 == 3);
    }
}

TEST_CASE("I2C read/write functions handle null device gracefully", "[i2c][raspi]")
{
    SECTION("i2c_write_byte handles null device")
    {
        disable_logging(WARNING);
        i2c_write_byte(nullptr, 0x00);
        enable_logging(WARNING);
        
        // Should not crash
        REQUIRE(true);
    }

    SECTION("i2c_write_register handles null device")
    {
        disable_logging(WARNING);
        i2c_write_register(nullptr, 0x00, 0x00, 1);
        enable_logging(WARNING);
        
        // Should not crash
        REQUIRE(true);
    }

    SECTION("i2c_read_byte handles null device")
    {
        disable_logging(WARNING);
        int result = i2c_read_byte(nullptr);
        enable_logging(WARNING);
        
        REQUIRE(result == -1);
    }

    SECTION("i2c_read_register handles null device")
    {
        disable_logging(WARNING);
        int result = i2c_read_register(nullptr, 0x00);
        enable_logging(WARNING);
        
        REQUIRE(result == -1);
    }
}

TEST_CASE("HT16K33 functions handle null device gracefully", "[i2c][ht16k33][raspi]")
{
    SECTION("ht16k33_init handles null device")
    {
        disable_logging(WARNING);
        ht16k33_init(nullptr);
        enable_logging(WARNING);
        
        // Should not crash
        REQUIRE(true);
    }

    SECTION("ht16k33_set_brightness handles null device")
    {
        disable_logging(WARNING);
        ht16k33_set_brightness(nullptr, 8);
        enable_logging(WARNING);
        
        // Should not crash
        REQUIRE(true);
    }

    SECTION("ht16k33_clear handles null device")
    {
        disable_logging(WARNING);
        ht16k33_clear(nullptr);
        enable_logging(WARNING);
        
        // Should not crash
        REQUIRE(true);
    }

    SECTION("ht16k33_draw_pattern handles null device")
    {
        unsigned char pattern[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
        
        disable_logging(WARNING);
        ht16k33_draw_pattern(nullptr, pattern);
        enable_logging(WARNING);
        
        // Should not crash
        REQUIRE(true);
    }

    SECTION("ht16k33_turn_off handles null device")
    {
        disable_logging(WARNING);
        ht16k33_turn_off(nullptr);
        enable_logging(WARNING);
        
        // Should not crash
        REQUIRE(true);
    }
}

#ifndef RASPBERRY_PI
TEST_CASE("I2C functions return appropriate errors on non-Pi platforms", "[i2c][raspi]")
{
    SECTION("open_i2c_device returns nullptr on non-Pi")
    {
        disable_logging(ERROR);
        i2c_device dev = open_i2c_device("test_device", 1, 0x70, I2C_DEVICE_HT16K33_8X8);
        enable_logging(ERROR);
        
        REQUIRE(dev == nullptr);
    }

    SECTION("open_i2c_device with defaults returns nullptr on non-Pi")
    {
        disable_logging(ERROR);
        i2c_device dev = open_i2c_device("test_device", I2C_DEVICE_HT16K33_8X8);
        enable_logging(ERROR);
        
        REQUIRE(dev == nullptr);
    }
}
#endif
