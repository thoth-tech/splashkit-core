/***********************************************
* XQuestCode || Aditya Parmar
* Code with Creativity
* 🚀 © 2024 Aditya Parmar. All Rights Reserved.
***********************************************/
#include <iostream>
#include "raspi_gpio.h"
using namespace std;
using namespace splashkit_lib;

// Function to run GPIO tests
void run_gpio_tests()
{
    // Initialize the GPIO
    cout << "Initializing GPIO" << endl;
    raspi_init();

    // Set GPIO pin 11 as an output
    cout << "Setting GPIO pin 11 as an output" << endl;
    raspi_set_mode(PIN_11, GPIO_OUTPUT);

    // Read the initial value of GPIO pin 11
    int defaultValue = raspi_read(PIN_11);
    cout << "Value of Pin 11: " << defaultValue << endl;

    // Write HIGH to GPIO pin 11
    cout << "Writing HIGH to GPIO pin 11" << endl;
    raspi_write(PIN_11, GPIO_HIGH);

    // Read the value of GPIO pin 11
    int value = raspi_read(PIN_11);
    cout << "GPIO 11 value: " << value << endl;

    // Write HIGH to GPIO pin 17
    cout << "Writing HIGH to GPIO pin 17" << endl;
    raspi_write(PIN_17, GPIO_HIGH);

    // Write HIGH to Ground PIN
    cout << "Writing HIGH to Ground PIN" << endl;
    raspi_write(PIN_6, GPIO_HIGH);

    // Clean up the GPIO
    raspi_cleanup();
}

void run_remote_gpio_tests()
{
    cout << "This test requires a remote Raspberry Pi running the Pigpio Daemon\n";
    cout << "Enter IP Address of remote Pi:\n";

    std::string host;
    std::getline(std::cin, host);

    // Initialize the GPIO
    cout << "Initializing GPIO" << endl;
    connection pi = remote_raspi_init("Raspi", host, 8888);

    // Set GPIO pin 11 as an output
    cout << "Setting GPIO pin 11 as an output" << endl;
    remote_raspi_set_mode(pi, PIN_11, GPIO_OUTPUT);

    // Read the initial value of GPIO pin 11
    int defaultValue = remote_raspi_read(pi, PIN_11);
    cout << "Value of Pin 11: " << defaultValue << endl;

    // Write HIGH to GPIO pin 11
    cout << "Writing HIGH to GPIO pin 11" << endl;
    remote_raspi_write(pi, PIN_11, GPIO_HIGH);

    // Read the value of GPIO pin 11
    int value = remote_raspi_read(pi, PIN_11);
    cout << "GPIO 11 value: " << value << endl;

    // Write HIGH to GPIO pin 17
    cout << "Writing HIGH to GPIO pin 17" << endl;
    remote_raspi_write(pi, PIN_17, GPIO_HIGH);

    // Write HIGH to Ground PIN
    cout << "Writing HIGH to Ground PIN" << endl;
    remote_raspi_write(pi, PIN_6, GPIO_HIGH);

    // Clean up the GPIO
    remote_raspi_cleanup(pi);
}
