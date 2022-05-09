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
const regex URL_pattern("^(https?:\/\/)?([\da-z\.-]+)\.([a-z\.]{2,6})([\/\w \.-]*)*\/?$");
const regex phone_number_pattern("^[\+]?[(]?[0-9]{3}[)]?[-\s\.]?[0-9]{3}[-\s\.]?[0-9]{4,6}$");
const regex email_pattern("[A-Z0-9a-z._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,64}");
const regex time_pattern("([0-9]|0[0-9]|1[0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])");




namespace splashkit_lib
{
    // Restrict decimal places to 3             [POSSIBLY REDUNDANT, C++ HAS ROUND FEATURE]
    string set_decimal_places(string s)
    {
        string temp;
        stringstream ss;

        temp = s.substr(s.find(".") + 1);

        if (temp.length() > 3)
        {
            write_line("Restricting to 3 decimals...");

            double value = convert_to_double(s);

            ss << fixed;
            ss << setprecision(3);
            ss << value;

            s = ss.str();

            write_line(s);
        }
        return s;
    }

    // Get custom filename                              [[[TO EXAMINE]]]
    string get_file_name(string original_file_path)
    {
        __fs::filesystem::path p(original_file_path);

        // Let user choose to keep or rename file
        write("Do you want to keep the file name? (y/n) ");
        string keep_file_name = read_line();
        string file_name;

        // Keep file name
        if (keep_file_name == "y")
        {
            file_name = p.filename();
        }
        // New file name
        else if (keep_file_name == "n")
        {
            write("New file name: ");
            file_name = read_line();
            file_name += p.extension();
        }
        // Invalid input
        else
        {
            write_line("Invalid input");
        }

        return file_name;
    }

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

    bool input_is_valid_url(string input) {
        return regex_match(input, URL_pattern);
    }

    bool input_is_email(string input) {
        return regex_match(input, email_pattern);
    }

    bool input_is_phone_number(string& input) {
        return regex_match(input, phone_number_pattern);
    }

    bool input_is_time(string input) {
        return regex_match(input, time_pattern);
    }

    // Provides a set of options for text validation. Returns input type.
    void text_validation(string input)
    {
        // Output menu with validation option
        write_line('== Text Validation ===\n1. Blank\n2. Email \n3. Phone number\n4. All\nSelect an option: ');

        int choice = convert_to_integer(read_line());

        // Switch validation option to cases
        switch (choice)
        {
        case 1:
            write_line('== Validating blank input ==');
            if (input.empty())
            {
                write_line('Input is empty.');
            }
            break;

        case 2:
            write_line('== Validating email ==');
            if (input_is_email(input))
            {
                write_line('Input is valid email.');
            }
            break;

        case 3:
            write_line('== Validating phone number ==');
            if (input_is_phone_number)
            {
                write_line('Input is a valid phone number.');
            }
            break;

        case 4:
            write_line('== Validating text with validation set ==');
            if (input_is_phone_number(input))
            {
                write_line('Input is phone number.');
            }
            else if (input_is_email)
            {
                write_line('Input is email.');
            }
            else if (input_is_time)
            {
                write_line('Input is time.');
            }
            else if (input_is_valid_url)
            {
                write_line('Input is a URL.');
            }
            else
            {
                write_line('Input does not pass validation checks.');
            }

        default:
            write_line('Your option is not valid! Please try again!');
        }
    }
}