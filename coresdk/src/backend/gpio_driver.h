// gpio_driver.h
// This file is part of the SplashKit Core Library.
// Copyright (©) 2024 Aditya Parmar. All Rights Reserved.

#ifndef SPLASHKIT_GPIO_H
#define SPLASHKIT_GPIO_H

#include "network_driver.h"
#include "backend_types.h"
#include <stdint.h> // Include the appropriate header file for stdint.h

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
    void sk_gpio_cleanup();
    #endif
    
    connection sk_remote_gpio_init(std::string name,const std::string &host, unsigned short int port);
    void sk_remote_gpio_set_mode(connection pi, int pin, int mode);
    int sk_remote_gpio_read(connection pi, int pin);
    void sk_remote_gpio_write(connection pi, int pin, int value);
    void sk_remote_clear_bank_1(connection pi);
    bool sk_remote_gpio_cleanup(connection pi);

    int sk_gpio_send_cmd(connection pi, sk_pigpio_cmd_t &cmd);

    void sk_gpio_package_command(sk_pigpio_cmd_t &cmd, char *buffer);
}

#endif /* defined(gpio_driver) */
