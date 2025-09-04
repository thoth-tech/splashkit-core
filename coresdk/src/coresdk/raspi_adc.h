/**
 * @header raspi_adc
 * @brief Provides support for using an ADC device with the GPIO pins on the Raspberry Pi.
 * @author Aditya Parmar
 * 
 * @attribute group  raspberry
 * @attribute static raspberry
 */

#ifndef raspi_adc_hpp
#define raspi_adc_hpp

#include "types.h"
#include <string>

namespace splashkit_lib
{
    /**
     * The `adc_device` type is used to refer to ADC (Analog-to-Digital Converter)
     * devices that can be managed by the SplashKit ADC code, such as ADS7830.
     * ADC devices are:
     *
     *   - loaded with `open_adc`,
     *   - accessed using `adc_device_named` or checked with `has_adc_device`,
     *   - read using `read_adc` to retrieve analog values from specific channels,
     *   - and must be closed using `close_adc` (to release a specific
     *     ADC device) or `close_all_adc` (to release all loaded ADC devices).
     *
     * ADC devices allow you to interface with external analog sensors or inputs,
     * converting their signals into digital values for processing in your application.
     *
     * @attribute class adc_device
     */
    typedef struct _adc_data *adc_device;

    /**
     * Checks if an ADC device with the given name has been loaded.
     *
     * @param name  The name used to identify the ADC device.
     * @returns     true if an ADC device with the supplied name exists.
     */
    bool has_adc_device(const string &name);

    /**
     * Retrieve an ADC device that has been loaded.
     *
     * @param name  The name of the ADC device.
     * @returns     The adc_device pointer if found; otherwise, nullptr.
     */
    adc_device adc_device_named(const string &name);

    /**
     * Loads an ADC device on the specified I2C bus at a given address.
     *
     * @param name    The name to assign this ADC device.
     * @param bus     The I2C bus number.
     * @param address The I2C address of the ADC device.
     * @param type    The type of ADC device (e.g., ADS7830, PCF8591).
     * @returns A valid adc_device on success, or nullptr on failure.
     *
     * @attribute class  adc_device
     * @attribute constructor true
     * @attribute suffix with_bus
     */
    adc_device open_adc(const string &name, int bus, int address, adc_type type);

    /**
     * Opens an ADC device with the specified name and type. Defaults to bus 1 and address 0x48.
     *
     * @param name  The name of the ADC device to open.
     * @param type  The type of ADC device (e.g., ADS7830, PCF8591).
     * @returns     A valid adc_device on success, or nullptr on failure.
     *
     * @attribute class        adc_device
     * @attribute constructor  true
     */
    adc_device open_adc(const string &name, adc_type type);

    /**
     * Reads an 8-bit value from the specified ADC channel on the device.
     *
     * @param adc      The ADC device to read from.
     * @param channel  The channel number to read (range depends on ADC type).
     * @returns        The ADC conversion value (0–255), or -1 on error.
     *
     * @attribute class   adc_device
     * @attribute self    adc
     * @attribute method  read
     */
    int read_adc(adc_device adc, adc_pin channel);

    /**
     * Reads an 8-bit value from the specified ADC channel on the device using its name.
     *
     * @param name     The ADC name string to close.
     * @param channel  The channel number to read (range depends on ADC type).
     * @returns        The ADC conversion value (0–255), or -1 on error.
     *
     * @attribute suffix  named
     */
    int read_adc(const string &name, adc_pin channel);

    /**
     * Closes an ADC device given its pointer.
     *
     * @param adc  The ADC device to close.
     *
     * @attribute class       adc_device
     * @attribute self        adc
     * @attribute destructor  true
     * @attribute method      close
     */
    void close_adc(adc_device adc);

    /**
     * Closes an ADC device given its name.
     *
     * @param name  The name of the ADC device to close.
     *
     * @attribute suffix  named
     */
    void close_adc(const string &name);

    /**
     * Closes all ADC devices that have been opened.
     */
    void close_all_adc();
    /**
     * @brief Initialises a remote connection to a Raspberry Pi.
     *
     * This function initialises a connection to a remote Raspberry Pi using the specified name, host, and port.
     *
     * @param name   The name of the connection.
     * @param host   The host address of the Raspberry Pi.
     * @param port   The port to use for the connection.
     * @returns      The connection object used to communicate with the remote Raspberry Pi.
     */
    connection remote_raspi_init(const string &name, const string &host, unsigned short int port);
	
	/**
     * @brief Sets the mode of the specified pin on a remote Raspberry Pi.
     *
     * This function sets the mode of a specific pin on a remote Raspberry Pi.
     *
     * @param pi     The connection object to the remote Raspberry Pi.
     * @param pin    The pin to set the mode for.
     * @param mode   The mode to set for the pin.
     */
    void remote_raspi_set_mode(connection pi, gpio_pin pin, gpio_pin_mode mode);
	
	/**
     * @brief Gets the mode of the specified pin on a remote Raspberry Pi.
     *
     * This function retrieves the mode of a specific pin on a remote Raspberry Pi.
     *
     * @param pi     The connection object to the remote Raspberry Pi.
     * @param pin    The pin to get the mode for.
     * @returns      The mode of the pin.
     */
    gpio_pin_mode remote_raspi_get_mode(connection pi, gpio_pin pin);
	
	/**
     * @brief Sets the pull up/down mode for the specified pin on a remote Raspberry Pi.
     *
     * This function sets the pull-up/down mode of a specific pin on a remote Raspberry Pi.
     *
     * @param pi     The connection object to the remote Raspberry Pi.
     * @param pin    The pin to set the pull up/down mode for.
     * @param pud    The pull up/down mode to set for the pin.
     */
    void remote_raspi_set_pull_up_down(connection pi, gpio_pin pin, pull_up_down pud);
	
	/**
     * @brief Writes a value to the specified pin on a remote Raspberry Pi.
     *
     * This function writes a specified value to a specific pin on a remote Raspberry Pi.
     *
     * @param pi     The connection object to the remote Raspberry Pi.
     * @param pin    The pin to write the value to.
     * @param value  The value to write to the pin.
     */
    void remote_raspi_write(connection pi, gpio_pin pin, gpio_pin_value value);
	
	/**
     * @brief Reads the value from the specified pin on a remote Raspberry Pi.
     *
     * This function reads the value from a specific pin on a remote Raspberry Pi.
     *
     * @param pi     The connection object to the remote Raspberry Pi.
     * @param pin    The pin to read the value from.
     * @returns      The value read from the pin.
     */
    gpio_pin_value remote_raspi_read(connection pi, gpio_pin pin);
}
#endif /* raspi_adc_hpp */
