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

void test_data_validation()
{
    // Output menu with validation options
    write_line("== Text Validation ===\n"
               "1. Email \n"
               "2. Phone number\n"
               "3. URL\n"
               "4. All\n"
               "Select an option: ");

    int choice = convert_to_integer(read_line());

    // Get input text to validate
    write_line("Enter text to validate:");
    string input = read_line();

    // Run different validations depending on choice
    switch (choice)
    {
        case 1:
        {
            write_line("== Validating email ==");
            if (input_is_email(input))
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
            write_line("== Validating phone number ==");
            if (input_is_phone_number(input))
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
            write_line("== Validating URL ==");
            if (input_is_valid_url(input))
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
            int input_type = identify_input_type(input);

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
        }
        default:
            write_line("Invalid option.");
    }
}

void run_data_analytics_test()
{
    write_line("== Data Analytics Module ==\n"
               "1. Data Validation\n"
               "Select an option: ");

    int test = convert_to_integer(read_line());

    switch (test)
    {
        case 1:
        {
            test_data_validation();
            break;
        }
        default:
        {
            write_line("Invalid option.");
        }
    }
}