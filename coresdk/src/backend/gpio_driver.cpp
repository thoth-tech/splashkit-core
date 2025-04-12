// gpio_driver.cpp
// This file is part of the SplashKit Core Library.
// Copyright (©) 2024 Aditya Parmar. All Rights Reserved.

#include "network_driver.h"
#include "gpio_driver.h"
#include "easylogging++.h"

#include <string>
#include <iostream>
#include <cstdlib> // Add this line to include the necessary header for the exit() function

#include <cstring>
#ifdef RASPBERRY_PI
#include "pigpiod_if2.h"
#include <chrono>
#include <thread>
#include <wiringPi.h>
#include <unordered_map>
#endif


using namespace std;
// Use https://abyz.me.uk/rpi/pigpio/pdif2.html for local command reference
//   Archive Link: https://web.archive.org/web/20240423160241/https://abyz.me.uk/rpi/pigpio/pdif2.html
//
// Use https://abyz.me.uk/rpi/pigpio/sif.html for remote command reference
//   Archive Link: https://web.archive.org/web/20240423160319/https://abyz.me.uk/rpi/pigpio/sif.html
namespace splashkit_lib
{
    #ifdef RASPBERRY_PI
    int pi = -1;
    int base_clock = 19200000;

    //Add map to track pin modes
    std::unordered_map<int, int> pin_modes;
    std::unordered_map<int, int> pwm_range;

    // Check if pigpio_init() has been called before any other GPIO functions
    bool check_pi()
    {
        if (pi < 0)
        {
            LOG(ERROR) << sk_gpio_error_message(pi);
            return false;
        }
        else return true;
    }

    // Initialize the GPIO library
    int sk_gpio_init()
    {
        if (wiringPiSetupGpio() == -1)
        {
            LOG(ERROR) << sk_gpio_error_message(pi);
            return 1;
        }
        pi = wiringPiSetupGpio();
        return pi;
    }

    // Read the value of a GPIO pin
    int sk_gpio_read(int pin)
    {
        if (check_pi())
        {
            //Checks whether the pins are in the correct range
            if (pin < 0 || pin > 40) 
            { 
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
                return -1;
            }
            int result = digitalRead(pin);
            //Verifies if a result is produced or not
            if (result < 0)
            {
                LOG(ERROR) << sk_gpio_error_message(result);
                return -1;
            }
            return result;
        }
        else
        {
            return GPIO_DEFAULT_VALUE;
        }
    }

    // Write a value to a GPIO pin
    void sk_gpio_write(int pin, int value)
    {
        if (check_pi())
        {
            //Checks whether the pins are in the correct range
            if (pin < 0 || pin > 40) 
            { 
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
                return;
            }
            //Checks if the value exists in the SplashKit library or not
            if (value < -1 || value > 7)
            {
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
                return;
            }
            digitalWrite(pin, value);
        }
    }

    // Set the mode of a GPIO pin
    void sk_gpio_set_mode(int pin, int mode)
    {
        if(check_pi())
        {
            //Checks whether the pins are in the correct range
            if (pin < 0 || pin > 40) 
            { 
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
                return;
            }
            //Checks if the value exists in the SplashKit library or not
            if (mode < 0 || mode > 7)
            {
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_MODE);
                return;
            }
            pinMode(pin, mode); 
            pin_modes[pin] = mode;
        }
    }

    // Get the mode of a GPIO pin
    int sk_gpio_get_mode(int pin)
    {
        if(check_pi())
        {
            //Checks whether the pins are in the correct range
            if (pin < 0 || pin > 40) 
            { 
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
                return -1;
            }
            int mode = pin_modes.count(pin) ? pin_modes[pin] : -1;
            return mode;
        }
        else
        {
            return PI_BAD_GPIO; 
        }
    }

    //Description
    void sk_gpio_set_pull_up_down(int pin, int pud)
    {
        //Checks whether the pins are in the correct range
        if(check_pi())
        {
            //Checks whether the pins are in the correct range
            if (pin < 0 || pin > 40) 
            { 
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
                return;
            }
            //Checks if the pud exists in the SplashKit library or not
            if (pud < 0 || pud > 2)
            {
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_PUD);
                return;
            }
            pinMode(pin, INPUT);
            pullUpDnControl(pin, pud);
        }
    }

    //Needs to be set before frequency and dutycycle
    void sk_set_pwm_range(int pin, int range)
    {
        if(check_pi())
        {
            //Checks whether the pins are in the correct range
            if (pin < 0 || pin > 40) 
            { 
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
                return;
            }
            //Checks whether newly set range is a reasonable value
            if (range <= 25 || range > 4096) 
            { 
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_DUTYRANGE);
                return;
            }
            pinMode(pin, PWM_OUTPUT); 
            pin_modes[pin] = PWM_OUTPUT;
            pwmSetMode(PWM_MODE_MS);
            pwmSetRange(range);
            pwm_range[pin] = range;
        }
    }

    // Find out what the clock divisor is using base clock, frequency and range
    // Set frequency by setting both the range & clock
    void sk_set_pwm_frequency(int pin, int frequency)
    {

        if(check_pi())
        {
            //Checks whether the pins are in the correct range
            if (pin < 0 || pin > 40) 
            { 
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
                return;
            }
            int range = pwm_range[pin];
            //Checks if range exists in the map of know PWM ranges
            if (range < 25)
            {
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_DUTYRANGE);
                return;
            }
            double divisor = static_cast<double>(base_clock) / (frequency * range);
            int clock_divisor = static_cast<int>(divisor + 0.5);
            //Checks if the new frequency is in a safe limit
            if ((range / clock_divisor) > 38400)
            {
                LOG(ERROR) << sk_gpio_error_message(-1);
                return;
            }
            pwmSetRange(range);
            pwmSetClock(clock_divisor);
        }
    }

    //Value must not be more than range (100% to 0%)
    void sk_set_pwm_dutycycle(int pin, int dutycycle)
    {
        if(check_pi())
        {
            //Checks whether the pins are in the correct range
            if (pin < 0 || pin > 40) 
            { 
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
                return;
            }
            int range = pwm_range[pin];
            //Checks if range exists in the map of know PWM ranges
            if (range < 25)
            {
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_DUTYRANGE);
                return;
            }
            //Check if dutycycle is less than range (percentage of cycle from 0 to 100% (range))
            else if (dutycycle > range)
            {
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_DUTYCYCLE);
                return;
            }
            pwmWrite(pin, dutycycle);
        }
    }

    void sk_gpio_clear_bank_1()
    {
        //clear_bank_1(pi, PI4B_GPIO_BITMASK);
        if(check_pi())
        {
            for (int pin = 0; gpio <= PI_SIZE; ++pin)
            {
                if (PI4B_GPIO_BITMASK && (1 << pin))
                {
                    int currentPin = pin;
                    pinMode(pin, OUTPUT);
                    digitalWrite(pin, LOW);
                }
            }
        }
    }

    // Cleanup the GPIO library
    //Delete function sk_gpio_cleanup
    void sk_gpio_cleanup()
    {
        if(check_pi())
        {
            pigpio_stop(pi);
        }
    }
    
    int sk_spi_open(int channel, int speed, int spi_flags)
    {
        //int fd = wiringPiSPISetup(channel, speed);
        if(check_pi())
            return spi_open(pi, channel, speed, spi_flags);
        else
            return -1;
    }

    int sk_spi_close(int handle)
    {
        //close(fd);
        
        if(check_pi())
            return spi_close(pi, handle);
        else
            return -1;
    }

    int sk_spi_transfer(int handle, char *send_buf, char *recv_buf, int count)
    {
        //wiringPiSPIDataRW(channel, buffer, length);
        if(check_pi())
            return spi_xfer(pi, handle, send_buf, recv_buf, count);
        else
            return -1;
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
        clear_bank_cmd.param1 = PI4B_GPIO_BITMASK;

        sk_gpio_send_cmd(pi, clear_bank_cmd);
    }

    bool sk_remote_gpio_cleanup(connection pi)
    {
        if(!is_connection_open(pi))
        {
            LOG(ERROR) << "Remote GPIO: Connection not open.";
            return false;
        }
        LOG(INFO) << "Cleaning Pins on Remote Pi Named: " << pi->name << endl;
        sk_remote_clear_bank_1(pi);
        return close_connection(pi);
    }

    int sk_gpio_send_cmd(connection pi, sk_pigpio_cmd_t &cmd)
    {
        if(!is_connection_open(pi))
        {
            LOG(ERROR) << sk_gpio_error_message(PIGIF_ERR_BAD_CONNECT); 
            return PIGIF_ERR_BAD_CONNECT;
        }

        if(pi->protocol == TCP)
        {
            int num_send_bytes = sizeof(cmd);

            std::vector<char> buffer(num_send_bytes);
            memcpy(buffer.data(), &cmd, num_send_bytes);

            if(sk_send_bytes(&pi->socket, buffer.data(), num_send_bytes)) 
            {
                int num_bytes_recv = sk_read_bytes(&pi->socket, buffer.data(), num_send_bytes); 
                if(num_bytes_recv == num_send_bytes) 
                {
                    sk_pigpio_cmd_t resp;
                    memcpy(&resp, buffer.data(), num_send_bytes);
                    
                    // We cast it back to a signed type so we can get the negative error codes.
                    int32_t result = static_cast<int32_t>(resp.result);

                    if (result < 0)
                    {
                        LOG(ERROR) << sk_gpio_error_message(result);
                    }

                    return result;
                }
                else
                {
                    LOG(ERROR) << sk_gpio_error_message(PIGIF_ERR_BAD_RECV);
                    return PIGIF_ERR_BAD_RECV;
                }
            }
            else
            {
                LOG(ERROR) << sk_gpio_error_message(PIGIF_ERR_BAD_SEND);
                return PIGIF_ERR_BAD_SEND;
            }
        }
        else
        {
            LOG(ERROR) << "Remote GPIO: Connection has UDP Protocol";
            return -1;
        }
    }
    
    std::string sk_gpio_error_message(int error_code)
    {
        switch (error_code)
        {
            case PI_INIT_FAILED:
                return "GPIO initialization failed. Please check your setup and try again.";
            case PI_BAD_USER_GPIO:
            case PI_BAD_GPIO:
                return "Invalid GPIO pin number.";
            case PI_BAD_MODE:
                return "Invalid GPIO mode. Valid modes are 0-7.";
            case PI_BAD_LEVEL:
                return "Invalid GPIO level. Valid levels are 0 (LOW) or 1 (HIGH).";
            case PI_BAD_PUD:
                return "Invalid pull-up/down configuration. Valid options are 0 (OFF), 1 (Pull-down), 2 (Pull-up).";
            case PI_BAD_DUTYCYCLE:
                return "Invalid PWM duty cycle. Duty cycle must be between 0 and the range value (default 255).";
            case PI_BAD_DUTYRANGE:
                return "Invalid PWM range. Range must be between 25 and 40000.";
            case PIGIF_ERR_BAD_SEND:
                return "Failed to send command to remote GPIO daemon (pigpiod).";
            case PIGIF_ERR_BAD_RECV:
                return "Failed to receive response from remote GPIO daemon (pigpiod).";
            case PIGIF_ERR_BAD_CONNECT:
                return "Failed to connect to remote GPIO daemon (pigpiod).";
            default:
                return "Unknown error code " + std::to_string(error_code);
        }
    }
}
