#include "data_analytics.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>
#include <regex>
#include "terminal.h"
#include "basics.h"

using namespace std;

namespace splashkit_lib
{
    // Save data into current file
    void save_file(string original_file_path, string data)
    {
        ofstream file(original_file_path);

        if (file.is_open())
        {
            file << data;
            file.close();
        }
        else
            cout << "Unable to save file";
    }

    // Input validation functions
    bool input_has_symbol(string input)
    {
        return regex_match(input, symbols_pattern);
    }

    bool input_has_alphabet_char(string input)
    {
        return regex_match(input, ABC_pattern);
    }

    bool input_is_valid_url(string input)
    {
        return regex_match(input, URL_pattern);
    }

    bool input_is_email(string input)
    {
        return regex_match(input, email_pattern);
    }

    bool input_is_phone_number(string input)
    {
        return regex_match(input, phone_number_pattern);
    }

    bool input_is_time(string input)
    {
        return regex_match(input, time_pattern);
    }

    // Run all validation checks attempting to identify and return input type
    int identify_input_type(string input)
    {
        if (input_is_phone_number(input))
        {
            return 1;
        }
        else if (input_is_email(input))
        {
            return 2;
        }
        else if (input_is_time(input))
        {
            return 3;
        }
        else if (input_is_valid_url(input))
        {
            return 4;
        }
        else
        {
            return -1;
        }
    }
}