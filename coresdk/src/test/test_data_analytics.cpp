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

void run_data_analytics_test()
{
    write_line("Enter text to validate:");
    string input = read_line();
    text_validation(input);
}
