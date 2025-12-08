//  raspi_i2c.cpp
//  splashkit
// This file is part of the SplashKit Core Library.
// SplashKit Team

#include "raspi_i2c.h"
#include "gpio_driver.h" // Provides sk_i2c_open, sk_i2c_close, sk_i2c_write_byte, etc.
#include "backend_types.h"
#include "easylogging++.h"
#include <string>
#include <map>

using std::map;
using std::string;

namespace splashkit_lib
{
    // Internal structure for the I2C device.
    struct _i2c_device_data
    {
        pointer_identifier id; // Should be I2C_DEVICE_PTR
        int i2c_handle;        // I2C handle (obtained from sk_i2c_open)
        int bus;               // I2C bus number
        int address;           // I2C address for the device
        i2c_device_type type;  // Device type
        string name;           // Device name
    };

    // Static map to manage loaded I2C devices (keyed by name)
    static map<string, i2c_device> _i2c_devices;

    // Default addresses for known device types
    static int _get_default_address(i2c_device_type type)
    {
        switch (type)
        {
        case I2C_DEVICE_HT16K33_8X8:
        case I2C_DEVICE_HT16K33_14SEG:
            return 0x70; // Default HT16K33 address
        case I2C_DEVICE_PCF8574:
            return 0x20; // Default PCF8574 address
        case I2C_DEVICE_GENERIC:
        default:
            return -1; // No default address for generic devices
        }
    }

    bool has_i2c_device(const string &name)
    {
        return _i2c_devices.count(name) > 0;
    }

    i2c_device i2c_device_named(const string &name)
    {
        if (has_i2c_device(name))
            return _i2c_devices[name];
        else
        {
            LOG(WARNING) << "I2C device not loaded with name: " << name;
            return nullptr;
        }
    }

    static i2c_device _load_i2c_device(const string &name, int bus, int address, i2c_device_type type)
    {
#ifdef RASPBERRY_PI
        if (has_i2c_device(name))
            return i2c_device_named(name);

        i2c_device result = new _i2c_device_data();
        result->id = I2C_DEVICE_PTR;
        result->bus = bus;
        result->address = address;
        result->name = name;
        result->type = type;

        // Open the I2C channel to the device
        result->i2c_handle = sk_i2c_open(bus, address);
        if (result->i2c_handle < 0)
        {
            LOG(WARNING) << "Error opening I2C device " << name
                         << " on bus " << bus << " at address 0x" << std::hex << address;
            delete result;
            return nullptr;
        }

        _i2c_devices[name] = result;
        LOG(INFO) << "Opened I2C device " << name << " on bus " << bus 
                  << " at address 0x" << std::hex << address;
        return result;
#else
        LOG(ERROR) << "I2C not supported on this platform";
        return nullptr;
#endif
    }

    i2c_device open_i2c_device(const string &name, int bus, int address, i2c_device_type type)
    {
#ifdef RASPBERRY_PI
        if (has_i2c_device(name))
        {
            LOG(WARNING) << "I2C device " << name << " already loaded.";
            return i2c_device_named(name);
        }
        return _load_i2c_device(name, bus, address, type);
#else
        LOG(ERROR) << "I2C not supported on this platform";
        return nullptr;
#endif
    }

    i2c_device open_i2c_device(const string &name, i2c_device_type type)
    {
#ifdef RASPBERRY_PI
        int default_address = _get_default_address(type);
        if (default_address < 0)
        {
            LOG(ERROR) << "No default address for device type, please specify address explicitly";
            return nullptr;
        }
        const int default_bus = 1;
        return _load_i2c_device(name, default_bus, default_address, type);
#else
        LOG(ERROR) << "I2C not supported on this platform";
        return nullptr;
#endif
    }

    void i2c_write_byte(i2c_device dev, int data)
    {
#ifdef RASPBERRY_PI
        if (dev == nullptr || dev->id != I2C_DEVICE_PTR)
        {
            LOG(WARNING) << "Invalid I2C device passed to i2c_write_byte";
            return;
        }
        sk_i2c_write_byte(dev->i2c_handle, data);
#else
        LOG(ERROR) << "I2C not supported on this platform";
#endif
    }

    void i2c_write_register(i2c_device dev, int reg, int data, int bytes)
    {
#ifdef RASPBERRY_PI
        if (dev == nullptr || dev->id != I2C_DEVICE_PTR)
        {
            LOG(WARNING) << "Invalid I2C device passed to i2c_write_register";
            return;
        }
        switch (bytes)
        {
        case 1:
            sk_i2c_write_byte_data(dev->i2c_handle, reg, data);
            break;
        case 2:
            sk_i2c_write_word_data(dev->i2c_handle, reg, data);
            break;
        default:
            LOG(ERROR) << "i2c_write_register: bytes must be 1 or 2";
            break;
        }
#else
        LOG(ERROR) << "I2C not supported on this platform";
#endif
    }

    int i2c_read_byte(i2c_device dev)
    {
#ifdef RASPBERRY_PI
        if (dev == nullptr || dev->id != I2C_DEVICE_PTR)
        {
            LOG(WARNING) << "Invalid I2C device passed to i2c_read_byte";
            return -1;
        }
        return sk_i2c_read_byte(dev->i2c_handle);
#else
        LOG(ERROR) << "I2C not supported on this platform";
        return -1;
#endif
    }

    int i2c_read_register(i2c_device dev, int reg)
    {
#ifdef RASPBERRY_PI
        if (dev == nullptr || dev->id != I2C_DEVICE_PTR)
        {
            LOG(WARNING) << "Invalid I2C device passed to i2c_read_register";
            return -1;
        }
        return sk_i2c_read_byte_data(dev->i2c_handle, reg);
#else
        LOG(ERROR) << "I2C not supported on this platform";
        return -1;
#endif
    }

    static void _close_i2c_device(i2c_device dev)
    {
#ifdef RASPBERRY_PI
        if (dev && dev->id == I2C_DEVICE_PTR)
        {
            sk_i2c_close(dev->i2c_handle);
            _i2c_devices.erase(dev->name);
            dev->id = NONE_PTR;
            delete dev;
        }
        else
        {
            LOG(WARNING) << "Attempting to close an invalid I2C device";
        }
#else
        LOG(ERROR) << "I2C not supported on this platform";
#endif
    }

    void close_i2c_device(i2c_device dev)
    {
#ifdef RASPBERRY_PI
        if (dev != nullptr)
            _close_i2c_device(dev);
        else
            LOG(WARNING) << "Attempted to close null I2C device";
#else
        LOG(ERROR) << "I2C not supported on this platform";
#endif
    }

    void close_i2c_device(const string &name)
    {
#ifdef RASPBERRY_PI
        i2c_device dev = i2c_device_named(name);
        close_i2c_device(dev);
#else
        LOG(ERROR) << "I2C not supported on this platform";
#endif
    }

    void close_all_i2c_devices()
    {
#ifdef RASPBERRY_PI
        // Create a copy of device names to avoid iterator invalidation
        vector<string> names;
        for (auto &entry : _i2c_devices)
        {
            names.push_back(entry.first);
        }
        for (auto &name : names)
        {
            close_i2c_device(name);
        }
#else
        LOG(ERROR) << "I2C not supported on this platform";
#endif
    }

    // ==================== HT16K33 LED Matrix Functions ====================

    // Helper to reverse bits (needed for LED matrix row data)
    static unsigned char _reverse_bits(unsigned char b)
    {
        b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
        b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
        b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
        return b;
    }

    void ht16k33_init(i2c_device dev)
    {
#ifdef RASPBERRY_PI
        if (dev == nullptr || dev->id != I2C_DEVICE_PTR)
        {
            LOG(WARNING) << "Invalid I2C device passed to ht16k33_init";
            return;
        }
        
        // Turn on system oscillator
        i2c_write_byte(dev, 0b00100001);
        
        // Set display on, no blink
        i2c_write_byte(dev, 0b10000001);
        
        // Clear display
        ht16k33_clear(dev);
        
        LOG(INFO) << "HT16K33 device " << dev->name << " initialized";
#else
        LOG(ERROR) << "I2C not supported on this platform";
#endif
    }

    void ht16k33_set_brightness(i2c_device dev, int brightness)
    {
#ifdef RASPBERRY_PI
        if (dev == nullptr || dev->id != I2C_DEVICE_PTR)
        {
            LOG(WARNING) << "Invalid I2C device passed to ht16k33_set_brightness";
            return;
        }
        
        // Clamp brightness to 0-15
        if (brightness < 0) brightness = 0;
        if (brightness > 15) brightness = 15;
        
        // Brightness command: 0xE0 | brightness value
        i2c_write_byte(dev, 0xE0 | brightness);
#else
        LOG(ERROR) << "I2C not supported on this platform";
#endif
    }

    void ht16k33_clear(i2c_device dev)
    {
#ifdef RASPBERRY_PI
        if (dev == nullptr || dev->id != I2C_DEVICE_PTR)
        {
            LOG(WARNING) << "Invalid I2C device passed to ht16k33_clear";
            return;
        }
        
        // Clear all 16 registers (8 rows * 2 bytes each for common cathode)
        for (int i = 0; i < 16; i++)
        {
            i2c_write_register(dev, i, 0, 1);
        }
#else
        LOG(ERROR) << "I2C not supported on this platform";
#endif
    }

    void ht16k33_draw_pattern(i2c_device dev, const unsigned char pattern[8])
    {
#ifdef RASPBERRY_PI
        if (dev == nullptr || dev->id != I2C_DEVICE_PTR)
        {
            LOG(WARNING) << "Invalid I2C device passed to ht16k33_draw_pattern";
            return;
        }
        
        for (int y = 0; y < 8; y++)
        {
            // Reverse and rotate bits to match LED matrix physical layout
            unsigned char row_data = _reverse_bits(pattern[y]);
            row_data = (row_data >> 1) | (row_data << 7);
            
            // Write to row register (each row is at address y*2)
            i2c_write_register(dev, y * 2, row_data, 1);
        }
#else
        LOG(ERROR) << "I2C not supported on this platform";
#endif
    }

    void ht16k33_turn_off(i2c_device dev)
    {
#ifdef RASPBERRY_PI
        if (dev == nullptr || dev->id != I2C_DEVICE_PTR)
        {
            LOG(WARNING) << "Invalid I2C device passed to ht16k33_turn_off";
            return;
        }
        
        // Turn off system oscillator
        i2c_write_byte(dev, 0b00100000);
#else
        LOG(ERROR) << "I2C not supported on this platform";
#endif
    }
}
