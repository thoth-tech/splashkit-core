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
#include "types.h"
using namespace std;
using namespace splashkit_lib;

void run_wiringPi_test()
{
    raspi_init();
    process_events();
    raspi_set_mode(PIN_11, GPIO_OUTPUT);
    raspi_get_mode(PIN_11);
    raspi_write(PIN_11, GPIO_HIGH);
    raspi_read(PIN_11);
    delay(5000);
    raspi_write(PIN_11, GPIO_LOW);
    raspi_set_pull_up_down(PIN_11, PUD_UP);  
    raspi_set_pull_up_down(PIN_11, PUD_DOWN);  
    raspi_set_pull_up_down(PIN_11, PUD_OFF);  
    // raspi_cleanup();
}