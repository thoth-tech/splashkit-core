// adc_driver.cpp
// This file is part of the SplashKit Core Library.
// Copyright (©) 2024 Aditya Parmar. All Rights Reserved.

#include "adc_driver.h"
#include "gpio_driver.h"         // Provides i2c_open, i2c_close, etc.
#include "backend_types.h"
#include "easylogging++.h"       // For logging
          

#include <string>
#include <map>
using std::map;
using std::string;

namespace splashkit_lib
{
    #ifdef RASPBERRY_PI
    // Internal structure for the ADC device.
    // We use ADC_PTR (defined in BackendTypes.h) to tag these pointers.
    struct _adc_data
    {
        pointer_identifier id; // Should be ADC_PTR
        int i2c_handle;        // I2C handle (obtained from i2c_open)
        int bus;               // I2C bus number
        int address;           // I2C address for the ADC device
        adc_type type;         // ADC type (e.g., ADS7830, PCF8591, etc.)
        string name;           // Device name
    };

    // Static map to manage loaded ADC devices (keyed by name)
    static map<string, adc_device> _adc_devices;

    bool has_adc_device(const string &name)
    {
        return _adc_devices.count(name) > 0;
    }
    // const int CMD_CH0 = 0x84;
   

    adc_device adc_device_named(const string &name)
    {
        if (has_adc_device(name))
            return _adc_devices[name];
        else
        {
            LOG(WARNING) << "ADC device not loaded with name: " << name;
            return nullptr;
        }
    }

    
    adc_device load_adc_device(const string &name, int bus, int address, adc_type type)
    {
        if (has_adc_device(name))
            return adc_device_named(name);

        adc_device result = new _adc_data();
        result->id = ADC_PTR;  // ADC_PTR is defined in backend_types.h (0x41444350)
        result->bus = bus;
        result->address = address;
        result->name = name;
        result->type = type;

        // Open the I2C channel to the ADC device.
        // (For both ADS7830 and PCF8591, we assume the initialization is similar.)
        result->i2c_handle = i2c_open(bus, address, 0);
        if (result->i2c_handle < 0)
        {
            LOG(WARNING) << "Error opening ADC device " << name
                         << " on bus " << bus << " at address " << address;
            delete result;
            return nullptr;
        }

        _adc_devices[name] = result;
        return result;
    }

    int read_adc_channel(adc_device dev, int channel)
    {
        if (dev == nullptr)
        {
            LOG(WARNING) << "Invalid ADC device.";
            return -1;
        }

        int command = 0;
        // Dispatch based on the ADC type.
        switch(dev->type)
        {
            case ADS7830:
            {
                // ADS7830 supports channels 0-7
                if (channel < 0 || channel > 7)
                {
                    LOG(WARNING) << "Invalid ADC channel: " << channel
                                 << " for device " << dev->name << " (ADS7830 supports 0-7)";
                    return -1;
                }
                command = channel;
                break;
            }
            // Uncomment and complete when implementing other ADC types.
            /*
            case PCF8591:
            {
                // Assuming PCF8591 supports channels 0-3; adjust validation as needed.
                if (channel < 0 || channel > 3)
                {
                    LOG(WARNING) << "Invalid ADC channel: " << channel
                                 << " for device " << dev->name << " (PCF8591 supports 0-3)";
                    return -1;
                }

                command = channel & 0x03;
                break;
            }
            */
            default:
                LOG(WARNING) << "Unsupported ADC type for device " << dev->name;
                return -1;
        }

        // Write the command byte to the device (selecting the channel and settings)
        i2c_write_byte(dev->i2c_handle, command);

        // Read the conversion result (8-bit value)
        int value = i2c_read_byte(dev->i2c_handle);
        if (value < 0)
        {
            LOG(WARNING) << "Error reading ADC channel " << channel
                         << " from device " << dev->name;
        }
        return value;
    }

    void free_adc_device(adc_device dev)
    {
        if (dev)
        {
            // Close the I2C connection
            i2c_close(dev->i2c_handle);
            // Remove the device from our map
            _adc_devices.erase(dev->name);
            dev->id = NONE_PTR;  // Set pointer to a non-valid identifier
            delete dev;
        }
        else
        {
            LOG(WARNING) << "Attempting to free an invalid ADC device";
        }
    }

    void free_all_adc_devices()
    {
        for (auto &entry : _adc_devices)
        {
            free_adc_device(entry.second);
        }
        _adc_devices.clear();
    }
    #endif
}
