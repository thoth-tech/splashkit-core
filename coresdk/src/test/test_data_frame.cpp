#include <string>
#include <iostream>

#include "data_frame.h"

using namespace splashkit_lib;
using std::cout, std::endl, std::vector, std::string, std::get;

void run_data_frame_test()
{
    data_frame df = create_data_frame();

    // Add column
    vector<data_element> col1 = {3, 4, 7};
    append_col(df, col1, "MyInts");

    // Insert column at start
    vector<data_element> col2 = {'A', 'B', 'C'};
    insert_col(df, 0, col2, "MyChars");

    // Add row
    vector<data_element> row1 = {'Z', 99};
    append_row(df, row1);

    // Extract element and parse to correct type
    data_element elem = get_cell(df, 0, 0);
    char parsed_elem = get<char>(elem);

    // Display
    display(df);

    cout << "Run './skunit_tests' to view unit tests" << endl;
}
