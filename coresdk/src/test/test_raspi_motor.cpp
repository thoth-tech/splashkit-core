/***********************************************
 * XQuestCode || Aditya Parmar
 * © 2024 Aditya Parmar. All Rights Reserved.
 ***********************************************/
#include <iostream>
#include "raspi_gpio.h"
#include "raspi_motor_driver.h"
#include "input.h"

using namespace std;
using namespace splashkit_lib;

void run_motor_driver_tests()
{
    process_events();
    cout << "=== L298N Motor Driver Test ===" << endl;
    // Initialize the Raspberry Pi GPIO subsystem
    raspi_init();

    // Instructions for wiring
    cout << "Wire your L298N driver as follows:" << endl;
    cout << "  IN1 -> GPIO24" << endl;
    cout << "  IN2 -> GPIO23" << endl;
    cout << "  ENA -> GPIO25 (PWM)" << endl;
    cout << "Press any key to begin..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Open the motor device
    motor_device dev = open_motor("Motor1", L298N, PIN_16, PIN_18, PIN_22);
    if (dev == nullptr)
    {
        cout << "ERROR: Failed to open motor device." << endl;
        raspi_cleanup();
        return;
    }

    // Run forward at 50% speed
    cout << "\nRunning motor FORWARD @ 50% speed. Press any key to reverse direction." << endl;
    set_motor_direction(dev, MOTOR_FORWARD);
    set_motor_speed(dev, 0.5);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Reverse at 50% speed
    cout << "\nReversing motor @ 50% speed. Press any key to brake (stop)." << endl;
    set_motor_direction(dev, MOTOR_BACKWARD);
    set_motor_speed(dev, 0.5);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Brake (both inputs HIGH)
    cout << "\nBraking motor. Press any key to clean up and exit." << endl;
    stop_motor(dev);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Clean up
    close_motor(dev);
    raspi_cleanup();

    cout << "\nMotor driver test completed." << endl;
}
