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

// REGEX PATTERNS
const regex symbols_pattern("^(?=.*[-+_!@#$%^&*., ?]).+$");
const regex ABC_pattern("^(?=.*[A-z]).+$");
const regex URL_pattern("^http[s]?:\/\/(www\.)?(.*)?\/?(.)*");
const regex phone_number_pattern("^[\+]?[(]?[0-9]{3}[)]?[-\s\.]?[0-9]{3}[-\s\.]?[0-9]{4,6}$");
const regex email_pattern("[A-Z0-9a-z._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,64}");
const regex time_pattern("([0-9]|0[0-9]|1[0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])");

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

    bool input_has_symbol(string input)
    {
        return regex_match(input, symbols_pattern);
    }

    bool input_has_char(string input)
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

    bool input_is_phone_number(string &input)
    {
        return regex_match(input, phone_number_pattern);
    }

    bool input_is_time(string input)
    {
        return regex_match(input, time_pattern);
    }
}