/**
 * @header raspi_i2c
 * @brief Provides support for communicating with I2C devices on the Raspberry Pi.
 * @author SplashKit Team
 *
 * @attribute group  raspberry
 * @attribute static raspberry
 */

#ifndef raspi_i2c_hpp
#define raspi_i2c_hpp

#include "types.h"
#include <string>

namespace splashkit_lib
{
    /**
     * The `i2c_device` type is used to refer to I2C devices that can be
     * managed by the SplashKit I2C code, such as LED matrices (HT16K33)
     * and other I2C peripherals.
     *
     * I2C devices are:
     *   - opened with `open_i2c_device`,
     *   - accessed using `i2c_device_named` or checked with `has_i2c_device`,
     *   - read/written using `i2c_read_byte`, `i2c_write_byte`, etc.,
     *   - and must be closed using `close_i2c_device` (to release a specific
     *     device) or `close_all_i2c_devices` (to release all loaded devices).
     *
     * @attribute class i2c_device
     */
    typedef struct _i2c_device_data *i2c_device;

    /**
     * Checks if an I2C device with the given name has been loaded.
     *
     * @param name  The name used to identify the I2C device.
     * @returns     true if an I2C device with the supplied name exists.
     */
    bool has_i2c_device(const string &name);

    /**
     * Retrieve an I2C device that has been loaded.
     *
     * @param name  The name of the I2C device.
     * @returns     The i2c_device pointer if found; otherwise, nullptr.
     */
    i2c_device i2c_device_named(const string &name);

    /**
     * Opens an I2C device on the specified bus at a given address.
     *
     * @param name    The name to assign this I2C device.
     * @param bus     The I2C bus number (usually 1 on Raspberry Pi).
     * @param address The I2C address of the device (e.g., 0x70 for HT16K33).
     * @param type    The type of I2C device.
     * @returns A valid i2c_device on success, or nullptr on failure.
     *
     * @attribute class  i2c_device
     * @attribute constructor true
     * @attribute suffix with_bus
     */
    i2c_device open_i2c_device(const string &name, int bus, int address, i2c_device_type type);

    /**
     * Opens an I2C device with the specified name and type using default bus and address.
     * Default bus is 1, default address depends on device type:
     *   - HT16K33: 0x70
     *   - PCF8574: 0x20
     *   - Generic: 0x00 (must be specified manually)
     *
     * @param name  The name of the I2C device to open.
     * @param type  The type of I2C device.
     * @returns     A valid i2c_device on success, or nullptr on failure.
     *
     * @attribute class        i2c_device
     * @attribute constructor  true
     */
    i2c_device open_i2c_device(const string &name, i2c_device_type type);

    /**
     * Writes a single byte to the I2C device.
     *
     * @param dev   The I2C device to write to.
     * @param data  The byte value to write.
     *
     * @attribute class   i2c_device
     * @attribute self    dev
     * @attribute method  write_byte
     */
    void i2c_write_byte(i2c_device dev, int data);

    /**
     * Writes data to a specific register on the I2C device.
     *
     * @param dev   The I2C device to write to.
     * @param reg   The register address to write to.
     * @param data  The data to write.
     * @param bytes The number of bytes to write (1 or 2).
     *
     * @attribute class   i2c_device
     * @attribute self    dev
     * @attribute method  write_register
     */
    void i2c_write_register(i2c_device dev, int reg, int data, int bytes);

    /**
     * Reads a single byte from the I2C device.
     *
     * @param dev  The I2C device to read from.
     * @returns    The byte value read, or -1 on error.
     *
     * @attribute class   i2c_device
     * @attribute self    dev
     * @attribute method  read_byte
     */
    int i2c_read_byte(i2c_device dev);

    /**
     * Reads data from a specific register on the I2C device.
     *
     * @param dev  The I2C device to read from.
     * @param reg  The register address to read from.
     * @returns    The byte value read, or -1 on error.
     *
     * @attribute class   i2c_device
     * @attribute self    dev
     * @attribute method  read_register
     */
    int i2c_read_register(i2c_device dev, int reg);

    /**
     * Closes an I2C device given its pointer.
     *
     * @param dev  The I2C device to close.
     *
     * @attribute class       i2c_device
     * @attribute self        dev
     * @attribute destructor  true
     * @attribute method      close
     */
    void close_i2c_device(i2c_device dev);

    /**
     * Closes an I2C device given its name.
     *
     * @param name  The name of the I2C device to close.
     *
     * @attribute suffix  named
     */
    void close_i2c_device(const string &name);

    /**
     * Closes all I2C devices that have been opened.
     */
    void close_all_i2c_devices();

    // ==================== HT16K33 LED Matrix Functions ====================

    /**
     * Initializes an HT16K33-based LED matrix device.
     * This turns on the system oscillator and enables the display.
     *
     * @param dev  The HT16K33 device to initialize.
     */
    void ht16k33_init(i2c_device dev);

    /**
     * Sets the brightness of an HT16K33-based LED device.
     *
     * @param dev        The HT16K33 device.
     * @param brightness The brightness level (0-15, where 15 is maximum).
     */
    void ht16k33_set_brightness(i2c_device dev, int brightness);

    /**
     * Clears the display on an HT16K33-based LED device.
     *
     * @param dev  The HT16K33 device to clear.
     */
    void ht16k33_clear(i2c_device dev);

    /**
     * Draws an 8-byte pattern on an 8x8 HT16K33 LED matrix.
     * Each byte represents one row of the display.
     *
     * @param dev      The HT16K33 device.
     * @param pattern  An 8-element array where each byte is a row pattern.
     */
    void ht16k33_draw_pattern(i2c_device dev, const unsigned char pattern[8]);

    /**
     * Turns off an HT16K33-based LED device by disabling the system oscillator.
     *
     * @param dev  The HT16K33 device to turn off.
     */
    void ht16k33_turn_off(i2c_device dev);
}
#endif /* raspi_i2c_hpp */
