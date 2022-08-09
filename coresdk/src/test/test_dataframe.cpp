#include <string>
#include <iostream>

#include "dataframe.h"

using namespace splashkit_lib;
using std::cout, std::endl, std::vector, std::string, std::get;

void run_dataframe_test()
{
    dataframe df = create_dataframe();

    // Add column
    vector<data_element> col1 = {3, 4, 7};
    dataframe_append_col(df, col1, "MyInts");

    // Insert column at start
    vector<data_element> col2 = {'A', 'B', 'C'};
    dataframe_insert_col(df, 0, col2, "MyChars");

    // Add row
    vector<data_element> row1 = {'Z', 99};
    dataframe_append_row(df, row1);

    // Extract element and parse to correct type
    data_element elem = dataframe_get_cell(df, 0, 0);
    char parsed_elem = get<char>(elem);

    // Display
    dataframe_display(df);

    cout << "Run './skunit_tests' to view unit tests" << endl;
}
