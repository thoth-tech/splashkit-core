//
//  test_animation.cpp
//  splashkit
//
//  Created by Andrew Cain on 12/08/2016.
//  Copyright © 2016 Andrew Cain. All rights reserved.
//

#include <iostream>
#include "networking.h"
#include "utils.h"
#include "input.h"
#include "raspi_gpio.h"
using namespace std;
using namespace splashkit_lib;

void run_wiringPi_test()
{
    raspi_init();
    process_events();
    // raspi_set_mode(PIN_11, GPIO_OUTPUT);
    // raspi_read(PIN_17);
    // raspi_write(PIN_11, GPIO_HIGH);
    // delay(5000);
    // raspi_write(PIN_11, GPIO_LOW);
    // raspi_cleanup();
}