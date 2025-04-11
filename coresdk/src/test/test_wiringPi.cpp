//
//  test_animation.cpp
//  splashkit
//
//  Created by Andrew Cain on 12/08/2016.
//  Copyright © 2016 Andrew Cain. All rights reserved.
//

#include <iostream>
#include "networking.h"
#include "raspi_gpio.h"
using namespace std;
using namespace splashkit_lib;

void run_wiringPi_test()
{
    raspi_init();
    raspi_read(PIN_11);
    raspi_write(PIN_11, GPIO_HIGH);
    raspi_write(PIN_11, GPIO_LOW);
}