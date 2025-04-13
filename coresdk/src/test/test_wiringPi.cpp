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
    cout << "Attempting to open SPI..." << endl;
    int handle = raspi_spi_open(0, 1000000);
    if(handle >= 0)
    {
        cout << "SPI Connection Opened" << endl;
    }
    else
    {
        cout << "SPI failed to open." << endl;
        return;
    }

    string buffer = "abc";
    int bytes_trans1 = 0;

    cout << "Attempting transfer..." << endl;
    string response = raspi_spi_transfer(handle, "abc", 2, bytes_trans1 );
    cout << bytes_trans1 << endl;     
    if(bytes_trans1 != -1)
    {
        cout << "Transfer error, wrong number of bytes transferred." << endl;     
    }
    cout << "Received " << response << endl;
}