// gpio_driver.h
// This file is part of the SplashKit Core Library.
// Copyright (©) 2024 Aditya Parmar. All Rights Reserved.

#ifndef SPLASHKIT_GPIO_H
#define SPLASHKIT_GPIO_H

#include "backend_types.h"
#include "types.h"
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

// Size of GPIO board
#define PI_SIZE 40

// Size of base clock
#define base_clock 19200000

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
    int sk_spi_open(int channel, int speed);
    int sk_spi_close(int handle);
    int sk_spi_transfer(int handle, char *buf, int count);
  
    #endif
    
    std::string sk_send_command(const std::string& ip, const std::string& user, const std::string& gpio_cmd);
    std::string get_hidden_input(const std::string& prompt);
    std::string get_user_input(const std::string& message);
    void sk_remote_gpio_init();
    void sk_remote_gpio_set_mode(int pin, int mode);
    int sk_remote_gpio_get_mode(int pin);
    void sk_remote_gpio_set_pull_up_down(int pin, int pud);
    int sk_remote_gpio_read(int pin);
    void sk_remote_gpio_write(int pin, int value);
    void sk_remote_set_pwm_range(int pin, int range);
    void sk_remote_set_pwm_frequency(int pin, int frequency);
    void sk_remote_set_pwm_dutycycle(int pin, int dutycycle);
    void sk_remote_clear_bank_1();
    bool sk_remote_gpio_cleanup();

    std::string sk_gpio_error_message(int error_code);
}

#endif /* defined(gpio_driver) */
