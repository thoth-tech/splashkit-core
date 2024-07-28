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
			pigpio_cmd_t set_cmd;
			set_cmd.cmd = 0;
			set_cmd.param1 = pin;
			set_cmd.param2 = mode;
			set_cmd.param3 = 0;
			set_cmd.res = 0;
			

			sk_gpio_send_cmd(pi, set_cmd);
		}

		int sk_remote_gpio_read(connection pi, int pin)
		{
			pigpio_cmd_t read_cmd;
			read_cmd.cmd = 3;
			read_cmd.param1 = pin;
			read_cmd.param2 = 0;
			read_cmd.param3 = 0;
			read_cmd.res = 0;

			return sk_gpio_send_cmd(pi, read_cmd);
		}

		void sk_remote_gpio_write(connection pi, int pin, int value)
		{
			pigpio_cmd_t write_cmd;
			write_cmd.cmd = 4;
			write_cmd.param1 = pin;
			write_cmd.param2 = value;
			write_cmd.param3 = 0;
			write_cmd.res = 0;

			sk_gpio_send_cmd(pi, write_cmd);
		}
		
		void sk_remote_clear_bank_1(connection pi)
		{
			pigpio_cmd_t clear_bank_cmd;
			clear_bank_cmd.cmd = 12;
			clear_bank_cmd.param1 = 0x0FFFFFFC;
			clear_bank_cmd.param2 = 0;
			clear_bank_cmd.param3 = 0;
			clear_bank_cmd.res = 0;
			
			sk_gpio_send_cmd(pi, clear_bank_cmd);
			
		}

		bool sk_remote_gpio_cleanup(connection pi)
		{
			sk_remote_clear_bank_1(pi);
			return close_connection(pi);
		}

		int sk_gpio_send_cmd(connection pi, pigpio_cmd_t &cmd)
		{
			if(!is_connection_open(pi)) 
			{ 
				std::cout << "Error, connection not open" << std::endl; 
				return -1; 
			}

			if(pi->protocol == TCP)
			{
				char buffer[sizeof(pigpio_cmd_t)];
				sk_gpio_package_command(cmd, buffer);


				if(sk_send_bytes(&pi->socket, buffer, sizeof(pigpio_cmd_t)))
				{
					char resp_buffer[sizeof(pigpio_cmd_t)];
					int bytes = sk_read_bytes(&pi->socket, resp_buffer, sizeof(pigpio_cmd_t));
					if(bytes == sizeof(pigpio_cmd_t))
					{
						pigpio_cmd_t resp;
						memcpy(&resp, resp_buffer, sizeof(pigpio_cmd_t));
						
						return resp.res;
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


    void sk_gpio_package_command(pigpio_cmd_t &cmd, char *buffer)
    {
		memcpy(buffer, &cmd, sizeof(cmd));
        /*memcpy(buffer, &cmd.cmd, sizeof(cmd.cmd));
        memcpy(buffer + sizeof(cmd.cmd), &cmd.param1, sizeof(cmd.param1));
        memcpy(buffer + sizeof(cmd.cmd) + sizeof(cmd.param1), &cmd.param2, sizeof(cmd.param2));
        memcpy(buffer + sizeof(cmd.cmd) + sizeof(cmd.param1) + sizeof(cmd.param2),
               &cmd.param3, sizeof(cmd.param3));*/
    }
}

