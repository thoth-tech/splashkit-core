// created by XQuestCode || Aditya Parmar
#include <iostream>
#include "raspi_gpio.h"
#include "input.h"

using namespace std;
using namespace splashkit_lib;

void run_gpio_adc_tests()
{

    cout << "Testing ADC with a ADS7830 and a potentiometer" << endl;
    raspi_init();
    cout << "Plug a potentiometer at A0 channel of the ADS7830" << endl;
    adc_device dev = open_adc("ADC1", 1, 0x48, ADS7830);
    if (dev == nullptr)
    {
        cout << "Failed to open ADC device." << endl;
        return;
    }

    adc_pin channel = ADC_PIN_0; // Change this to the desired channel
    int value = 0;
    while (!any_key_pressed())
    {
        value = read_adc(dev, channel);
        cout << "ADC value: " << value << endl;
    }

    close_adc(dev);
    raspi_cleanup();
    cout << "ADC test completed." << endl;
}