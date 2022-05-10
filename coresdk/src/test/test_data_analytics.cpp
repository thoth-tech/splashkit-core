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

// Provides a set of options for text validation.
void text_validation(string input)
{
    // Output menu with validation options
    write_line("== Text Validation ===\n1. Blank\n2. Email \n3. Phone number\n4. URL\n5. All\nSelect an option: ");

    int choice = convert_to_integer(read_line());

    // Run different validations depending on choice
    switch (choice)
    {
    case 1:
        write_line("== Validating blank input ==");
        if (input.empty())
        {
            write_line("Input is empty.");
        }
        else
        {
            write_line("Input is invalid.");
        }
        break;

    case 2:
        write_line("== Validating email ==");
        if (input_is_email(input))
        {
            write_line("Input is valid email.");
        }
        else
        {
            write_line("Input is invalid.");
        }
        break;

    case 3:
        write_line("== Validating phone number ==");
        if (input_is_phone_number(input))
        {
            write_line("Input is a valid phone number.");
        }
        else
        {
            write_line("Input is invalid.");
        }
        break;

    case 4:
        write_line("== Validating URL ==");
        if (input_is_valid_url(input))
        {
            write_line("Input is a valid URL");
        }
        else
        {
            write_line("Input is invalid.");
        }
        break;

    case 5:
        write_line("== Validating text with validation set ==");
        if (input_is_phone_number(input))
        {
            write_line("Input is a phone number.");
            break;
        }
        else if (input_is_email(input))
        {
            write_line("Input is an email address.");
            break;
        }
        else if (input_is_time(input))
        {
            write_line("Input is time.");
            break;
        }
        else if (input_is_valid_url(input))
        {
            write_line("Input is a URL.");
            break;
        }
        else
        {
            write_line("Input does not match any validation tests.");
        }
    }
}

void run_data_analytics_test()
{
    write_line("Enter text to validate:");
    string input = read_line();
    text_validation(input);
}
