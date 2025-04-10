#ifndef SPLASHKIT_ADC_H
#define SPLASHKIT_ADC_H

#include "backend_types.h"
#include <string>

namespace splashkit_lib
{

    // Define the ADC device type using a pointer to an internal structure
    typedef struct _adc_data *adc_device;
#ifdef RASPBERRY_PI
    /**
     * Checks if an ADC device with the given name has been loaded.
     *
     * @param name The name used to identify the ADC device.
     * @returns true if an ADC device with the supplied name exists.
     */
    bool has_adc_device(const std::string &name);

    /**
     * Retrieve an ADC device that has been loaded.
     *
     * @param name The name of the ADC device.
     * @returns The adc_device pointer if found; otherwise, nullptr.
     */
    adc_device adc_device_named(const std::string &name);

    /**
     * Loads an ADC device on the specified I2C bus at a given address.
     *
     * @param name    The name to assign this ADC device.
     * @param bus     The I2C bus number.
     * @param address The I2C address of the ADC device.
     * @param type    The type of ADC device (e.g., ADS7830, PCF8591).
     * @returns A valid adc_device on success, or nullptr on failure.
     */
    adc_device load_adc_device(const std::string &name, int bus, int address, adc_type type);

    /**
     * Reads an 8-bit value from the specified ADC channel on the device.
     * For instance, ADS7830 supports channels 0-7.
     *
     * @param dev     The adc_device to use.
     * @param channel The channel number to read (range depends on ADC type).
     * @returns The ADC conversion value (0–255), or -1 on error.
     */
    int read_adc_channel(adc_device dev, int channel);

    /**
     * Frees the resources associated with an ADC device.
     *
     * @param dev The adc_device to free.
     */
    void free_adc_device(adc_device dev);

    /**
     * Frees all loaded ADC devices.
     */
    void free_all_adc_devices();
#endif
}

#endif // SPLASHKIT_ADC_H
