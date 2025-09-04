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
#include "networking.h"
#include <string>

namespace splashkit_lib
{
    // Forward declare adc_device as a pointer to internal struct _adc_data
    typedef struct _adc_data* adc_device;

    typedef struct _remote_adc_data* remote_adc_device;


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
     * Loads an ADC device on the specified I2C bus at a given address remotely.
     *
     * @param name The name to assign this ADC device.
     * @param bus The I2C bus number.
     * @param address The I2C address of the ADC device.
     * @param type The type of ADC device (e.g., ADS7830, PCF8591).
     * @returns A valid adc_device on success, or nullptr on failure.
     */
    remote_adc_device remote_open_adc(connection pi, string name, int bus, int address, adc_type type);

    /**
     * Opens an ADC device with the specified name and type using remote gpio.
     * Defaults to bus 1 and address 0x48.
     *
     * @param name The name of the ADC device to open.
     * @param type The type of ADC device (e.g., ADS7830, PCF8591).
     * @returns A valid adc_device on success, or nullptr on failure.
     */
    remote_adc_device remote_open_adc(connection pi, string name, adc_type type);

    /**
     * Reads an 8-bit value from the specified ADC channel on the device on remote gpio.
     *
     * @param adc The ADC device to read from.
     * @param channel The channel number to read (range depends on ADC type).
     * @returns The ADC conversion value (0–255), or -1 on error.
     */
    int remote_read_adc(remote_adc_device adc, adc_pin channel);

    /**
     * Reads an 8-bit value from the specified ADC channel on the device using its name.
     *
     * @param name The ADC device name.
     * @param channel The channel number to read (range depends on ADC type).
     * @returns The ADC conversion value (0–255), or -1 on error.
     */
    int remote_read_adc(const std::string &name, adc_pin channel);

    /**
     * Closes an ADC device given its pointer.
     *
     * @param adc The ADC device to close.
     */
    void remote_close_adc(remote_adc_device adc);

    /**
     * Closes an ADC device given its name.
     *
     * @param name The name of the ADC device to close.
     */
    void remote_close_adc(const std::string &name);

    /**
     * Closes a specific ADC device that have been opened.
     */
    void remote_close_adc_device(remote_adc_device dev);
}
#endif /* raspi_adc_hpp */
