#include <iostream>
#include "input.h"
#include "data_analytics.h"
#include "terminal.h"
#include "basics.h"
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>
#include <regex>

using namespace std;
using namespace splashkit_lib;

// Uses SplaskKit inbuilt functions to ensure input is able to be converted to integer as non-integer conversions broke the program.
int integer_convert_check(string s)
{
    if (is_integer(s))
    { 
        return convert_to_integer(s);
    }
    write_line("Input was not an integer");
    return -1; 
}


void test_data_validation()
{
    // Output menu with validation options
    write_line("== Text Validation ===\n"
               "1. Email \n"
               "2. Phone number\n"
               "3. URL\n"
               "4. All\n"
               "Select an option: ");

    int choice = integer_convert_check(read_line());

    // Run different validations depending on choice
    switch (choice)
    {
        case 1:
        {
            write_line("== Validating email ==\n");

            // Get input text to validate
            write_line("Enter text to validate:");
            if (input_is_email(read_line()))
            {
                write_line("Input is valid email.");
            }
            else
            {
                write_line("Input is not a valid email.");
            }
            break;
        }

        case 2:
        {
            write_line("== Validating phone number ==\n");

            // Get input text to validate
            write_line("Enter text to validate:");
            if (input_is_phone_number(read_line()))
            {
                write_line("Input is a valid phone number.");
            }
            else
            {
                write_line("Input is not a valid phone number.");
            }
            break;
        }
        case 3:
        {
            write_line("== Validating URL ==\n");

            // Get input text to validate
            write_line("Enter text to validate:");
            if (input_is_valid_url(read_line()))
            {
                write_line("Input is a valid URL");
            }
            else
            {
                write_line("Input is not a valid URL.");
            }
            break;
        }

        // Run all validation tests in attempt to identify input type.
        case 4:
        {
            write_line("== Validating text with validation set ==");

            // Get input text to validate
            write_line("Enter text to validate:");
            int input_type = identify_input_type(read_line());

            switch (input_type)
            {
                case 1:
                    write_line("Input is a phone number.");
                    break;

                case 2:
                    write_line("Input is an email address.");
                    break;

                case 3:
                    write_line("Input is time.");
                    break;

                case 4:
                    write_line("Input is a URL.");
                    break;

                case -1:
                    write_line("Input does not match any validation tests.");
                    break;
            }
            break;
        }
        default:
            write_line("Invalid option.");
    }
}


void run_data_analytics_test()
{
    int test = 0;
    do {
        write_line("== Data Analytics Module ==\n"
        "-1. Quit Data Analytics Module\n"
        "0. Data Validation\n"
        "Select an option: ");

        
        test = integer_convert_check(read_line());

        switch (test)
        {
            case -1:
            {
                write_line("Return to SplashKit Test Main");
                break;
            }
            case 0:
            {
                test_data_validation();
                break;
            }
            default:
            {
                write_line("Invalid option.");
                break;
            }    
        }

    } while (test != -1);
    
}