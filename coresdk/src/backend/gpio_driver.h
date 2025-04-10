// gpio_driver.h
// This file is part of the SplashKit Core Library.
// Copyright (©) 2024 Aditya Parmar. All Rights Reserved.

#ifndef SPLASHKIT_GPIO_H
#define SPLASHKIT_GPIO_H

#include "backend_types.h"
#include <stdint.h> // Include the appropriate header file for stdint.h

// Relevant error codes from pigpio library
#define PI_INIT_FAILED             -1
#define PI_BAD_USER_GPIO           -2
#define PI_BAD_GPIO                -3
#define PI_BAD_MODE                -4
#define PI_BAD_LEVEL               -5
#define PI_BAD_PUD                 -6
#define PI_BAD_DUTYCYCLE           -8
#define PI_BAD_DUTYRANGE           -21
#define PI_NOT_PERMITTED           -41
#define PI_SOME_PERMITTED          -42
#define PIGIF_ERR_BAD_SEND         -2000
#define PIGIF_ERR_BAD_RECV         -2001
#define PIGIF_ERR_BAD_CONNECT      -2003

// Bitmask for valid user GPIO on the 4B board
#define PI4B_GPIO_BITMASK 0x0FFFFFFC

namespace splashkit_lib
{

    #ifdef RASPBERRY_PI
  
    int sk_gpio_init();
    int sk_gpio_read(int pin);
    void sk_gpio_set_mode(int pin, int mode);
    int sk_gpio_get_mode(int pin);
    void sk_gpio_set_pull_up_down(int pin, int pud);
    void sk_gpio_write(int pin, int value);
    void sk_set_pwm_range(int pin, int range);
    void sk_set_pwm_frequency(int pin, int frequency);
    void sk_set_pwm_dutycycle(int pin, int dutycycle);
    void sk_gpio_clear_bank_1();
    int sk_spi_open(int channel, int speed, int spi_flags);
    int sk_spi_close(int handle);
    int sk_spi_transfer(int handle, char *sendBuf, char *recvBuf, int count);

    // I2C Functions
    void i2c_open(int bus, int address, int flags);
    void i2c_close(int handle);
    int i2c_read_byte(int handle);
    void i2c_write_byte(int handle, int data);
    int i2c_read_device(int handle, char *buf, int count);
    void i2c_write_device(int handle, char *buf, int count);

    // Additional I2C Functions
    int sk_i2c_read_byte_data(int handle, int reg);
    void sk_i2c_write_byte_data(int handle, int reg, int data);
    int sk_i2c_read_word_data(int handle, int reg);
    void sk_i2c_write_word_data(int handle, int reg, int data);

    void sk_gpio_cleanup();
  
    #endif
    
    connection sk_remote_gpio_init(std::string name,const std::string &host, unsigned short int port);
    void sk_remote_gpio_set_mode(connection pi, int pin, int mode);
    int sk_remote_gpio_get_mode(connection pi, int pin);
    void sk_remote_gpio_set_pull_up_down(connection pi, int pin, int pud);
    int sk_remote_gpio_read(connection pi, int pin);
    void sk_remote_gpio_write(connection pi, int pin, int value);
    void sk_remote_set_pwm_range(connection pi, int pin, int range);
    void sk_remote_set_pwm_frequency(connection pi, int pin, int frequency);
    void sk_remote_set_pwm_dutycycle(connection pi, int pin, int dutycycle);
    void sk_remote_clear_bank_1(connection pi);
    bool sk_remote_gpio_cleanup(connection pi);

    int sk_gpio_send_cmd(connection pi, sk_pigpio_cmd_t &cmd);
    std::string sk_gpio_error_message(int error_code);
}

#endif /* defined(gpio_driver) */
