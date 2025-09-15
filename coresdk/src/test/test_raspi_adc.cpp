//
//  test_raspi_adc.cpp
//  splashkit
//

#include <iostream>
#include "raspi_gpio.h"
#include "raspi_adc.h"
#include "input.h"
#include "input_driver.h"

using namespace std;
using namespace splashkit_lib;

void run_potentiometer_test(gpio_pin end_test_button)
{
    cout << "ADC Test 1: Using a Potentiometer with ADC device: ADS7830" << endl;
    cout << "- Connect a potentiometer to A0 channel on the ADS7830 device" << endl;

    adc_device dev = open_adc("ADC1", 1, 0x48, ADS7830);
    if (dev == nullptr)
    {
        cout << "Failed to open ADC device." << endl;
        return;
    }

    adc_pin channel = ADC_PIN_0; // Can change this to the desired channel
    int value = 0;

    while (raspi_read(end_test_button) != GPIO_HIGH)
    {
        value = read_adc(dev, channel);
        cout << "ADC value: " << value << endl;
    }

    close_adc(dev);
    cout << "ADC potentiometer test completed." << endl;
}

void run_joystick_test(gpio_pin end_test_button)
{
    cout << "Test 2: Testing ADC (ADS7830) using a Joystick module." << endl;
    cout << "- Connect VRx to A1 channel on the ADS7830 device." << endl;
    cout << "- Connect VRy to A2 channel on the ADS7830 device." << endl;

    adc_device adc = open_adc("ADC1", 1, 0x48, ADS7830);
    if (adc == nullptr)
    {
        cout << "Failed to open ADC device." << endl;
        return;
    }

    // Joystick movement values
    int joystick_x;
    int joystick_y;

    while (raspi_read(end_test_button) != GPIO_HIGH)
    {
        joystick_x = read_adc(adc, ADC_PIN_1);
        joystick_y = read_adc(adc, ADC_PIN_2);

        cout << "\033[2J\033[H" << endl;
        cout << "Joystick values:" << endl;
        cout << "  VRx: " << joystick_x << "\tVRy: " << joystick_y << endl;
    }

    // Clean up
    close_adc(adc);
}


void run_remote_potentiometer_test(connection pi, gpio_pin end_test_button)
{
    cout << "ADC Test 1: Using a Potentiometer with ADC device: ADS7830" << endl;
    cout << "- Connect a potentiometer to A0 channel on the ADS7830 device" << endl;

    remote_adc_device dev = remote_open_adc(pi, "ADC1", 1, 0x48, ADS7830);
    if (dev == nullptr)
    {
        cout << "Failed to open ADC device." << endl;
        return;
    }

    adc_pin channel = ADC_PIN_0; // Can change this to the desired channel
    int value = 0;

    while (remote_raspi_read(pi, end_test_button) != GPIO_HIGH)
    {
        value = remote_read_adc(dev, channel);
        cout << "ADC value: " << value << endl;
    }

    remote_close_adc(dev);
    cout << "ADC potentiometer test completed." << endl;
}

void run_remote_joystick_test(connection pi, gpio_pin end_test_button)
{
    cout << "Test 2: Testing ADC (ADS7830) using a Joystick module." << endl;
    cout << "- Connect VRx to A1 channel on the ADS7830 device." << endl;
    cout << "- Connect VRy to A2 channel on the ADS7830 device." << endl;

    remote_adc_device adc = remote_open_adc(pi, "ADC1", 1, 0x48, ADS7830);
    if (adc == nullptr)
    {
        cout << "Failed to open ADC device." << endl;
        return;
    }

    // Joystick movement values
    int joystick_x;
    int joystick_y;

    while (remote_raspi_read(pi, end_test_button) != GPIO_HIGH)
    {
        joystick_x = remote_read_adc(adc, ADC_PIN_1);
        joystick_y = remote_read_adc(adc, ADC_PIN_2);

        cout << "\033[2J\033[H" << endl;
        cout << "Joystick values:" << endl;
        cout << "  VRx: " << joystick_x << "\tVRy: " << joystick_y << endl;
    }

    // Clean up
    remote_close_adc(adc);
}

void run_gpio_adc_tests()
{
    const int NUMBER_OF_ADC_TESTS = 2; // Update this if more tests are added

    cout << endl;
    cout << "ADC Tests:" << endl;
    cout << endl;
    cout << "Note: The following GPIO tests require a button to wired to pin 7 to allow the test to be stopped cleanly." << endl;
    cout << endl;

    int user_input = 0;
    while (user_input < 1 || user_input > NUMBER_OF_ADC_TESTS)
    {
        cout << "ADC Tests using ADC device: ADS7830" << endl;
        cout << "1: Potentiometer test" << endl;
        cout << "2: Joystick test" << endl;
        cout << "------------------------" << endl;
        cout << "Select ADC test to run: ";
        cin >> user_input;
        cin.ignore();
    }

    // Initialise GPIO & I2C
    raspi_init();

    // Set up button for ending the program
    gpio_pin end_button_pin = PIN_7;
    raspi_set_mode(end_button_pin, GPIO_INPUT);
    raspi_set_pull_up_down(end_button_pin, PUD_DOWN);

    switch (user_input)
    {
    case 1:
        run_potentiometer_test(end_button_pin);
        break;
    case 2:
        run_joystick_test(end_button_pin);
        break;
    default:
        break;
    }

    // Clean up
    raspi_cleanup();
    cout << "ADC test complete." << endl;
}

void run_remote_gpio_adc_tests()
{
    const int NUMBER_OF_ADC_TESTS = 2; // Update this if more tests are added

    cout << endl;
    cout << "ADC Tests:" << endl;
    cout << endl;
    cout << "Note: The following GPIO tests require a button to wired to pin 7 on the remote Pi to allow the test to be stopped cleanly." << endl;
    cout << endl;

    cout << "This test requires a remote Raspberry Pi running the Pigpio Daemon" << endl;
    cout << "Enter IP Address of remote Pi:" << endl;

    std::string host;
    std::getline(std::cin, host);

    cout << "Initializing GPIO" << endl;
    connection pi = remote_raspi_init("Raspi", host, 8888);

    int user_input = 0;
    while (user_input < 1 || user_input > NUMBER_OF_ADC_TESTS) {
        cout << "ADC Tests using ADC device: ADS7830" << endl;
        cout << "1: Potentiometer test" << endl;
        cout << "2: Joystick test" << endl;
        cout << "------------------------" << endl;
        cout << "Select ADC test to run: ";
        cin >> user_input;
        cin.ignore();
    }

    // Set up button on the remote Pi for ending the program
    gpio_pin end_button_pin = PIN_7;
    remote_raspi_set_mode(pi, end_button_pin, GPIO_INPUT);
    remote_raspi_set_pull_up_down(pi, end_button_pin, PUD_DOWN);

    switch (user_input) {
    case 1:
        run_remote_potentiometer_test(pi, end_button_pin);
        break;
    case 2:
        run_remote_joystick_test(pi, end_button_pin);
        break;
    default:
        break;
    }

    // Clean up remote connection
    remote_raspi_cleanup(pi);
    cout << "ADC test complete." << endl;
}