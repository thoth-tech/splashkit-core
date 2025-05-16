/***********************************************
 * XQuestCode || Aditya Parmar
 * © 2024 Aditya Parmar. All Rights Reserved.
 ***********************************************/
#include <iostream>
#include <limits>
#include <unistd.h>            // for usleep()
#include "raspi_gpio.h"
#include "raspi_servo_driver.h"

using namespace std;
using namespace splashkit_lib;

void run_servo_driver_tests()
{
    cout << "=== Servo Sweep Test ===" << endl;
    // Initialize GPIO subsystem
    raspi_init();

    // Wiring instructions
    cout << "Wire your servo as follows:\n"
         << "  Signal → BOARD pin 12 (GPIO18)\n"
         << "  VCC    → Pi 5 V (pin 2 or 4)\n"
         << "  GND    → Pi GND (e.g. pin 6)\n\n";

    // Pause until user is ready
    cout << "Press ENTER to begin..." << flush;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Open the servo on board pin 12
    servo_device srv = open_servo("TestServo", PIN_12);
    if (!srv) {
        cout << "ERROR: Failed to open servo device." << endl;
        raspi_cleanup();
        return;
    }

    const int   STEP_DEG  = 10;      // degrees per step
    const useconds_t DELAY = 20000;  // 20 ms per step

    // Sweep from 0° → 180°
    cout << "Sweeping 0° → 180°..." << endl;
    for (int deg = 0; deg <= 180; deg += STEP_DEG) {
        set_servo_angle(srv, deg);
        usleep(DELAY);
    }

    // Sweep from 180° → 0°
    cout << "Sweeping 180° → 0°..." << endl;
    for (int deg = 180; deg >= 0; deg -= STEP_DEG) {
        set_servo_angle(srv, deg);
        usleep(DELAY);
    }

    // Stop pulses and clean up
    stop_servo(srv);
    close_servo(srv);
    raspi_cleanup();

    cout << "\nServo sweep test completed." << endl;
}

