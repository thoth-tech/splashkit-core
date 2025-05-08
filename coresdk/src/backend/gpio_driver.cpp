// gpio_driver.cpp
// This file is part of the SplashKit Core Library.
// Copyright (©) 2024 Aditya Parmar. All Rights Reserved.

#include "network_driver.h"
#include "gpio_driver.h"
#include "easylogging++.h"

#include <string>
#include <iostream>
#include <cstdlib> // Add this line to include the necessary header for the exit() function

#include <unistd.h>
#include <cstring>
#ifdef RASPBERRY_PI
#include <wiringPi.h>
#include <unordered_map>
#include <wiringPiSPI.h>

#endif

#ifdef _WIN32
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif



using namespace std;
// Use https://abyz.me.uk/rpi/pigpio/pdif2.html for local command reference
//   Archive Link: https://web.archive.org/web/20240423160241/https://abyz.me.uk/rpi/pigpio/pdif2.html
//
// Use https://abyz.me.uk/rpi/pigpio/sif.html for remote command reference
//   Archive Link: https://web.archive.org/web/20240423160319/https://abyz.me.uk/rpi/pigpio/sif.html
namespace splashkit_lib
{
    //Add map to track items for remote gpio
    std::unordered_map<int, int> r_pin_modes;
    std::unordered_map<int, int> r_pwm_range;

    int base_clock = 19200000;

    std::string username;
    std::string ip;

    #ifdef RASPBERRY_PI
    int pi = -1;
    //Add map to track items for RPi GPIO
    std::unordered_map<int, int> pin_modes;
    std::unordered_map<int, int> pwm_range;
    std::unordered_map<int, int> handle_channel;

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
            if (value < -1 || value > 2)
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
            else if (range < dutycycle)
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
            // Manually go through each pin and reset it to 0 (LOW)
            for (int pin = 0; pin <= PI_SIZE; ++pin)
            {
                if (PI4B_GPIO_BITMASK && (1 << pin))
                {
                    int currentPin = pin;
                    pinMode(pin, OUTPUT);
                    digitalWrite(pin, LOW);
                    pin_modes[pin] = LOW;
                }
            }
        }
    }

    //Delete function sk_gpio_cleanup
    
    // WiringPi's version of spi_open doesn't need the variable flag so I removed it
    int sk_spi_open(int channel, int speed)
    {
        if(check_pi())
        {
            //Checks whether the channel is in the correct range
            if (channel < 0 || channel > 2) 
            { 
                LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
                return -1;
            }
            int handle = wiringPiSPISetup(channel, speed);
            //Save handle to unordered map
            handle_channel[handle] = channel;
            return handle;
        }
        else
        {
            return -1;
        }
    }

    int sk_spi_close(int handle)
    {
        if(check_pi())
        {
            //Close SPI & reset handle value to 0
            close(handle); 
            handle_channel[handle] = 0;
            return 0;
        }
        else
        {
            return -1;
        }
    }

    //This function uses a combined buffer now called buf
    int sk_spi_transfer(int handle, char *buf, int count)
    {
        if(check_pi())
        {
            //If handle is -1, it doesn't existzzz
            if (handle == -1)
            {
                return -1;
            }
            unsigned char* u_buf = (unsigned char*) buf;
            int channel = handle_channel[handle];
            //Checks whether the channel is in the correct range or if it's not 0
            if (channel >= 0 || channel < 2)
            {
                return -1;
            }
            int val = wiringPiSPIDataRW(channel, u_buf, count);
            return val;
        }
        else
        {
            return -1;
        }
    }	

    #endif


    std::string sk_send_command(const std::string& ip, const std::string& user, const std::string& gpio_cmd, const std::string& path) {
        // Construct the full plink command using PowerShell

        #ifdef _WIN32
            std::string command = "C:\\msys64\\usr\\bin\\bash.exe -lc \"ssh " + user + "@" + ip + " '" + gpio_cmd + "'\"";
        #else
            std::string command = "ssh " + user + "@" + ip + " '" + gpio_cmd + "'";
        #endif

    
        std::cout << "Running command: " << command << std::endl;
        std::array<char, 128> buffer;
        std::string result;
        #ifdef _WIN32
            #define popen _popen
            #define pclose _pclose
        #endif
    
        // Use popen to capture output
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
        if (!pipe) {
            std::cerr << "Failed to run command." << std::endl;
            return "";
        }

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
    
        return result;
    }    
    
    std::string get_user_input(const std::string& message) {
        while (true) {
            std::string input;
            std::cout << message << ": ";
            std::getline(std::cin, input);
            if (!input.empty()) return input;
            std::cout << "Input cannot be empty. Try again.\n";
        }
    }
    
    void sk_remote_gpio_init() {
        std::string temp_ip = get_user_input("Provide the Raspberry Pi's IP Address");
        std::string temp_user = get_user_input("Provide the Raspberry Pi's Username");
        
        username = temp_user;
        ip = temp_ip;
    }
    
    // // Wrapper functions
    void sk_remote_gpio_set_mode(int pin, int mode) {
        // Checks whether the pins are in the correct range
        if (pin < 0 || pin > 40) 
        { 
            LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
            return;
        }
    
        // Checks if the mode is valid
        if (mode < 0 || mode > 7)
        {
            LOG(ERROR) << sk_gpio_error_message(PI_BAD_MODE);
            return;
        }
    
        // Construct the correct GPIO mode string
        std::string mode_str;
        switch (mode) {
            case 0: mode_str = "input"; break;
            case 1: mode_str = "output"; break;
            default: 
                return;
        }
    
        // Send the command to set pin mode
        std::string output = sk_send_command(ip, username, "gpio -g mode " + std::to_string(pin) + " " + mode_str, path);
    
        std::cout << "Pin: " << pin << " Mode: " << mode_str << std::endl;
        r_pin_modes[pin] = mode;
    }
    
    
    int sk_remote_gpio_get_mode(int pin) {
        //Checks whether the pins are in the correct range
        if (pin < 0 || pin > 40) 
        { 
            LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
            return -1;
        }
        int mode = r_pin_modes.count(pin) ? r_pin_modes[pin] : -1;
        std::cout<<mode;
        return mode;
    }
    
    void sk_remote_gpio_write(int pin, int value) {
        if (pin < 0 || pin > 40) 
        { 
            LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
            return;
        }
        if (value < 0 || value > 1) 
        { 
            LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
            return;
        }
        int mode = sk_remote_gpio_get_mode(pin);
        if (mode != 1)
        {
            return;
        }
        std::string output = sk_send_command(ip, username, "gpio -g write " + std::to_string(pin) + " " + std::to_string(value), path);
    }
    
    int sk_remote_gpio_read(int pin) {
        if (pin < 0 || pin > 40) 
        { 
            LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
            return -1;
        }
        std::string output = sk_send_command(ip, username, "gpio -g read " + std::to_string(pin), path);
        
        // Trim output (in case of newline or extra whitespace)
        output.erase(std::remove_if(output.begin(), output.end(), ::isspace), output.end());
    
    
        if (output == "1") return 1;
        if (output == "0") return 0;
    
        std::cerr << "Unexpected GPIO read result: " << output << std::endl;
        // Indicate error
        return -1; 
    }
    
    
    void sk_remote_gpio_set_pull_up_down(int pin, int pud) {
        if (pin < 0 || pin > 40) 
        { 
            LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
            return;
        }
        if (pud < 0 || pud > 1) 
        { 
            LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
            return;
        }
        std::string pud_str;
        switch (pud) {
            case 0: pud_str = "down"; break;
            case 1: pud_str = "up"; break;
            default: 
                return;
        }
        std::string output = sk_send_command(ip, username, "gpio -g mode " + std::to_string(pin) + " " + pud_str, path);
    }
    
    void setup_pwm(int pin)
    {
        std::string output = sk_send_command(ip, username, "gpio -g mode " + std::to_string(pin) + " pwm", path);
    }
    
    void sk_remote_set_pwm_range(int pin, int range) {
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
        int mode = sk_remote_gpio_get_mode(pin);
        //num for pwm
        if (mode != GPIO_PWM)
        {
            setup_pwm(pin);        
            r_pin_modes[pin] = GPIO_PWM;
        }
        std::string output = sk_send_command(ip, username, "gpio -g pwmr " + std::to_string(range), path);
        r_pwm_range[pin] = range;
    }
    
    void sk_remote_set_pwm_frequency(int pin, int frequency) {
        if (pin < 0 || pin > 40) 
        { 
            LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
            return;
        }
        int range = r_pwm_range[pin];
        //Checks if range exists in the map of know PWM ranges
        if (range < 25)
        {
            LOG(ERROR) << sk_gpio_error_message(PI_BAD_DUTYRANGE);
            return;
        }
        double divisor = static_cast<double>(base_clock) / (frequency * range);
        int clock_divisor = static_cast<int>(divisor + 0.5);
        int mode = sk_remote_gpio_get_mode(pin);
        //num for pwm
        if (mode != GPIO_PWM)
        {
            setup_pwm(pin);        
            r_pin_modes[pin] = GPIO_PWM;
        }
        std::string output = sk_send_command(ip, username, "gpio pwm-ms", path);
    output = sk_send_command(ip, username, "gpio -g pwmc " + std::to_string(clock_divisor), path);
        r_pwm_range[pin] = range;
        r_pin_modes[pin] = 2;
    }
    
    void sk_remote_set_pwm_dutycycle(int pin, int dutycycle) {
        if (pin < 0 || pin > 40) 
        { 
            LOG(ERROR) << sk_gpio_error_message(PI_BAD_GPIO);
            return;
        }
        int range = r_pwm_range[pin];
        //Checks if range exists in the map of know PWM ranges
        if (range < 25)
        {
            //LOG(ERROR) << sk_gpio_error_message(PI_BAD_DUTYRANGE);
            return;
        }
        int mode = sk_remote_gpio_get_mode(pin);
        //num for pwm
        if (mode != GPIO_PWM)
        {
            setup_pwm(pin);        
            r_pin_modes[pin] = GPIO_PWM;
        }
        std::string output = sk_send_command(ip, username, "gpio -g pwm " + std::to_string(pin) + " " + std::to_string(dutycycle), path);
        r_pwm_range[pin] = range;
    }
    
    void sk_remote_clear_bank_1() {
        // Manually go through each pin and reset it to 0 (LOW)
        for (int pin = 0; pin <= 40; ++pin)
        {
            if (PI4B_GPIO_BITMASK && (1 << pin))
            {
                int currentPin = pin;
                r_pin_modes[pin] = GPIO_LOW;
                r_pwm_range[pin] = GPIO_LOW;
            }
        }
    }

    bool sk_remote_gpio_cleanup() {
        std::cout << "Cleaning up remote GPIO...\n";
        sk_remote_clear_bank_1();
        username = "";
        password = "";
        ip = "";
        path = "";
        return true;
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
