#include <string>
#include <iostream>

#include "dataframe.h"

using namespace splashkit_lib;
using std::cout, std::endl, std::vector, std::string, std::get;

void run_dataframe_test()
{
    dataframe df = create_dataframe();

    // Add column
    cout << "Append integer column {3, 4, 7} called 'MyInts' to dataframe" << endl;
    vector<data_element> col1 = {3, 4, 7};
    dataframe_append_col(df, col1, "MyInts");
    dataframe_display(df);

    // Insert column at start
    cout << "\nInsert character column with a null element {A, null, C} called 'MyChars' at front of dataframe" << endl;
    vector<data_element> col2 = {'A', DATAFRAME_NULL, 'C'};
    dataframe_insert_col(df, 0, col2, "MyChars");
    dataframe_display(df);

    // Update column
    cout << "\nUpdate integer column 'MyInts' with {9, 8, 7} and rename to 'MyInts2' in dataframe" << endl;
    vector<data_element> col3 = {9, 8, 7};
    dataframe_update_col(df, 1, col3, "MyInts2");
    dataframe_display(df);

    // Add row
    cout << "\nAppend row {Z, 99} to dataframe" << endl;
    vector<data_element> row1 = {'Z', 99};
    dataframe_append_row(df, row1);
    dataframe_display(df);

    // Extract element and parse to correct type
    data_element elem = dataframe_get_cell(df, 0, 0);
    char parsed_elem = get<char>(elem);
    cout << "\nExtract element in the first row and column in dataframe" << endl;
    cout << parsed_elem << endl;

    // Extract row
    cout << "\nExtract second column in dataframe" << endl;
    dataframe_display_col(df, 1);

    // Extract column
    cout << "\nExtract third row in dataframe" << endl;
    dataframe_display_row(df, 2);

    // Count rows
    cout << "\nGet number of columns in dataframe" << endl;
    cout << dataframe_num_cols(df) << endl;

    // Count columns
    cout << "\nGet number of rows in dataframe" << endl;
    cout << dataframe_num_rows(df) << endl;

    // Column types
    cout << "\nGet column types in dataframe" << endl;
    for (string col_type : dataframe_get_col_types(df))
        cout << col_type << " ";
    cout << endl;

    // Column names
    cout << "\nGet column names in dataframe" << endl;
    for (string col_name : dataframe_get_col_names(df))
        cout << col_name << " ";
    cout << endl;

    // Delete rows
    cout << "\nDelete second row in dataframe" << endl;
    dataframe_delete_row(df, 1);
    dataframe_display(df);

    // Delete column
    cout << "\nDelete first column in dataframe" << endl;
    dataframe_delete_col(df, 0);
    dataframe_display(df);

    cout << "\nRun './skunit_tests' to view unit tests" << endl;
}
