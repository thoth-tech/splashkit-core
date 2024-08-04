// gpio_driver.cpp
// This file is part of the SplashKit Core Library.
// Copyright (©) 2024 Aditya Parmar. All Rights Reserved.

#include "gpio_driver.h"


#include <string>
#include <iostream>
#include <cstdlib> // Add this line to include the necessary header for the exit() function

#include <cstring>

#ifdef RASPBERRY_PI
#include "pigpiod_if2.h"
#endif


using namespace std;
// Use https://abyz.me.uk/rpi/pigpio/pdif2.html for reference
namespace splashkit_lib
{
        #ifdef RASPBERRY_PI
        int pi;

        // Check if pigpio_init() has been called before any other GPIO functions
        bool check_pi()
        {

                if (pi < 0)
                {
                        cout << "gpio_init() must be called before any other GPIO functions" << endl;
                        return false; 
                }
                else
                        return true;
        }

        // Initialize the GPIO library
        int sk_gpio_init()
        {

                pi = pigpio_start(0, 0);
                return pi;
        }

        // Read the value of a GPIO pin
        int sk_gpio_read(int pin)
        {

                if (check_pi())
                {
                        return gpio_read(pi, pin);
                }
        }

        // Write a value to a GPIO pin
        void sk_gpio_write(int pin, int value)
        {

                check_pi();
                gpio_write(pi, pin, value);
        }

        // Set the mode of a GPIO pin
        void sk_gpio_set_mode(int pin, int mode)
        {

                check_pi();
                set_mode(pi, pin, mode);
        }

        int sk_gpio_get_mode(int pin)
        {

                check_pi();
                return get_mode(pi, pin);
        }
        void sk_gpio_set_pull_up_down(int pin, int pud)
        {

                check_pi();
                set_pull_up_down(pi, pin, pud);
        }
        void sk_set_pwm_range(int pin, int range)
        {

                check_pi();
                set_PWM_range(pi, pin, range);
        }
        void sk_set_pwm_frequency(int pin, int frequency)
        {

                check_pi();
                set_PWM_frequency(pi, pin, frequency);
        }
        void sk_set_pwm_dutycycle(int pin, int dutycycle)
        {

                check_pi();
                set_PWM_dutycycle(pi, pin, dutycycle);
        }

        // Cleanup the GPIO library
        void sk_gpio_cleanup()
        {

                check_pi();

                pigpio_stop(pi);
        }
        #endif
        
        connection sk_remote_gpio_init(std::string name, const std::string &host, unsigned short int port)
        {
            return open_connection(name, host, port);
        }

        void sk_remote_gpio_set_mode(connection pi, int pin, int mode)
        {
            sk_pigpio_cmd_t set_cmd;
            set_cmd.cmd_code = GPIO_CMD_SET_MODE; 
            set_cmd.param1 = pin;
            set_cmd.param2 = mode;
            

            sk_gpio_send_cmd(pi, set_cmd);
        }

        int sk_remote_gpio_get_mode(connection pi, int pin)
        {
            sk_pigpio_cmd_t get_cmd;
            get_cmd.cmd_code = GPIO_CMD_GET_MODE;
            get_cmd.param1 = pin;

            return sk_gpio_send_cmd(pi, get_cmd);
        }

        void sk_remote_gpio_set_pull_up_down(connection pi, int pin, int pud)
        {
            sk_pigpio_cmd_t set_pud_cmd;
            set_pud_cmd.cmd_code = GPIO_CMD_SET_PUD;
            set_pud_cmd.param1 = pin;
            set_pud_cmd.param2 = pud;

            sk_gpio_send_cmd(pi, set_pud_cmd);
        }

        int sk_remote_gpio_read(connection pi, int pin)
        {
            sk_pigpio_cmd_t read_cmd;
            read_cmd.cmd_code = GPIO_CMD_READ;
            read_cmd.param1 = pin;

            return sk_gpio_send_cmd(pi, read_cmd);
        }

        void sk_remote_gpio_write(connection pi, int pin, int value)
        {
            sk_pigpio_cmd_t write_cmd;
            write_cmd.cmd_code = GPIO_CMD_WRITE;
            write_cmd.param1 = pin;
            write_cmd.param2 = value;

            sk_gpio_send_cmd(pi, write_cmd);
        }

        void sk_remote_set_pwm_range(connection pi, int pin, int range)
        {
            sk_pigpio_cmd_t set_range_cmd;
            set_range_cmd.cmd_code = GPIO_CMD_SET_PWM_RANGE;
            set_range_cmd.param1 = pin;
            set_range_cmd.param2 = range;

            sk_gpio_send_cmd(pi, set_range_cmd);
        }

        void sk_remote_set_pwm_frequency(connection pi, int pin, int frequency)
        {
            sk_pigpio_cmd_t set_freq_cmd;
            set_freq_cmd.cmd_code = GPIO_CMD_SET_PWM_FREQ;
            set_freq_cmd.param1 = pin;
            set_freq_cmd.param2 = frequency;

            sk_gpio_send_cmd(pi, set_freq_cmd);
        }

        void sk_remote_set_pwm_dutycycle(connection pi, int pin, int dutycycle)
        {
            sk_pigpio_cmd_t set_dutycycle_cmd;
            set_dutycycle_cmd.cmd_code = GPIO_CMD_SET_PWM_DUTYCYCLE;
            set_dutycycle_cmd.param1 = pin;
            set_dutycycle_cmd.param2 = dutycycle;

            sk_gpio_send_cmd(pi, set_dutycycle_cmd);
        }
        
        void sk_remote_clear_bank_1(connection pi)
        {
            sk_pigpio_cmd_t clear_bank_cmd;
            clear_bank_cmd.cmd_code = GPIO_CMD_CLEAR_BANK_1;
            clear_bank_cmd.param1 = 0x0FFFFFFC;
            
            sk_gpio_send_cmd(pi, clear_bank_cmd);
            
        }

        bool sk_remote_gpio_cleanup(connection pi)
        {
            sk_remote_clear_bank_1(pi);
            return close_connection(pi);
        }

        int sk_gpio_send_cmd(connection pi, sk_pigpio_cmd_t &cmd)
        {
            if(!is_connection_open(pi)) 
            { 
                std::cout << "Error, connection not open" << std::endl; 
                return -1; 
            }

            if(pi->protocol == TCP)
            {
                char buffer[sizeof(sk_pigpio_cmd_t)];
                memcpy(buffer, &cmd, sizeof(cmd));

                if(sk_send_bytes(&pi->socket, buffer, sizeof(sk_pigpio_cmd_t)))
                {
                    char resp_buffer[sizeof(sk_pigpio_cmd_t)];
                    int bytes = sk_read_bytes(&pi->socket, resp_buffer, sizeof(sk_pigpio_cmd_t));
                    if(bytes == sizeof(sk_pigpio_cmd_t))
                    {
                        sk_pigpio_cmd_t resp;
                        memcpy(&resp, resp_buffer, sizeof(sk_pigpio_cmd_t));
                        
                        return resp.result;
                    }
                    else
                    {
                        std::cout << "Error: Failed to read response from socket" << std::endl;
                    }
                }
                else
                {
                    std::cout << "Error: Failed to send command to socket." << std::endl;
                }
            }
            else
            {
                std::cout << "Error: connection has UDP protocol" << std::endl;
            }
        }
}

