#include "catch.hpp"
#include "dataframe.h"
#include <stdexcept>

using namespace splashkit_lib;
using std::vector, std::string, std::get;

dataframe create_demo_dataframe()
{
    dataframe df = create_dataframe();
    vector<data_element> demo_col1 = {9, 4, 8};
    vector<data_element> demo_col2 = {'A', 'B', 'C'};
    vector<data_element> demo_col3 = {true, false, true};
    vector<data_element> demo_col4 = {1.1f, 2.2f, 3.3f};
    vector<data_element> demo_col5 = {"Aa", "Bb", "Cc"};
    dataframe_append_col(df, demo_col1, "Col 1");
    dataframe_append_col(df, demo_col2, "Col 2");
    dataframe_append_col(df, demo_col3, "Col 3");
    dataframe_append_col(df, demo_col4, "Col 4");
    dataframe_append_col(df, demo_col5, "Col 5");
    return df;
}

TEST_CASE( "Dataframe", "[dataframe]" )
{
    SECTION( "Creating a dataframe" )
    {
        dataframe df = create_dataframe();
        REQUIRE( dataframe_num_rows(df) == 0 );
        REQUIRE( dataframe_num_cols(df) == 0 );
    }

    SECTION( "Getting number of rows" )
    {
        dataframe df = create_demo_dataframe();
        REQUIRE( dataframe_num_rows(df) == 3 );
    }

    SECTION( "Getting number of columns" )
    {
        dataframe df = create_demo_dataframe();
        REQUIRE( dataframe_num_cols(df) == 5 );
    }

    SECTION( "Getting columns" )
    {
        dataframe df = create_demo_dataframe();

        SECTION( "Getting valid columns")
        {
            vector<data_element> demo_col = dataframe_get_col(df, 0);
            REQUIRE( get<int>(demo_col[0]) == 9 );
            REQUIRE( get<int>(demo_col[2]) == 8 );

            demo_col = dataframe_get_col(df, 4);
            REQUIRE( get<string>(demo_col[0]) == "Aa" );
            REQUIRE( get<string>(demo_col[1]) == "Bb" );
        }

        SECTION( "Getting invalid column indexes")
        {
            REQUIRE_THROWS_AS( dataframe_get_col(df, -1), std::out_of_range );
            REQUIRE_THROWS_AS( dataframe_get_col(df, 5), std::out_of_range );
        }
    }

    SECTION( "Getting rows" )
    {
        dataframe df = create_demo_dataframe();

        SECTION( "Getting valid rows")
        {
            vector<data_element> demo_row = dataframe_get_row(df, 0);
            REQUIRE( get<char>(demo_row[1]) == 'A' );
            REQUIRE( get<float>(demo_row[3]) == 1.1f );

            demo_row = dataframe_get_row(df, 2);
            REQUIRE( get<char>(demo_row[1]) == 'C' );
            REQUIRE( get<float>(demo_row[3]) == 3.3f );
        }

        SECTION( "Getting invalid row indexes")
        {
            REQUIRE_THROWS_AS( dataframe_get_row(df, -1), std::out_of_range );
            REQUIRE_THROWS_AS( dataframe_get_row(df, 3), std::out_of_range );
        }
    }

    SECTION( "Getting cells" )
    {
        dataframe df = create_demo_dataframe();

        SECTION( "Getting valid cells" )
        {
            REQUIRE( get<string>(dataframe_get_cell(df, 1, 4)) == "Bb");
            REQUIRE( get<int>(dataframe_get_cell(df, 2, 0)) == 8);
        }

        SECTION( "Getting invalid cell indexes" )
        {
            REQUIRE_THROWS_AS( dataframe_get_cell(df, -1, 1), std::out_of_range );
            REQUIRE_THROWS_AS( dataframe_get_cell(df, 1, -1), std::out_of_range );
            REQUIRE_THROWS_AS( dataframe_get_cell(df, 1, 5), std::out_of_range );
            REQUIRE_THROWS_AS( dataframe_get_cell(df, 3, 1), std::out_of_range );
        }
    }

    SECTION( "Appending columns" )
    {
        dataframe df = create_demo_dataframe();

        SECTION( "Appending valid columns")
        {
            vector<data_element> demo_col = {-1, -2, -3};
            dataframe_append_col(df, demo_col, "Col 6");
            vector<data_element> extract_col = dataframe_get_col(df, 5);
            REQUIRE( get<int>(extract_col[0]) == -1 );
            REQUIRE( get<int>(extract_col[2]) == -3 );

            demo_col = {'Z', 'Y', 'X'};
            dataframe_append_col(df, demo_col, "Col 7");
            extract_col = dataframe_get_col(df, 6);
            REQUIRE( get<char>(extract_col[0]) == 'Z');
            REQUIRE( get<char>(extract_col[2]) == 'X');
        }

        SECTION( "Appending incorrect length column" )
        {
            vector<data_element> demo_col = {1, 2};
            REQUIRE_THROWS_AS( dataframe_append_col(df, demo_col, "Col 6"), std::invalid_argument );
        }

        SECTION( "Appending column with variable types" )
        {
            vector<data_element> demo_col = {1, '2', 3};
            REQUIRE_THROWS_AS( dataframe_append_col(df, demo_col, "Col 6"), std::invalid_argument );
        }
    }

    SECTION( "Appending rows" )
    {
        dataframe df = create_demo_dataframe();

        SECTION( "Appending valid rows" )
        {
            vector<data_element> demo_row = {-1, 'Z', false, -1.1f, "Zz"};
            dataframe_append_row(df, demo_row);
            vector<data_element> extract_row = dataframe_get_row(df, 3);
            REQUIRE( get<int>(extract_row[0]) == -1 );
            REQUIRE( get<float>(extract_row[3]) == -1.1f );

            demo_row = {-2, 'Y', true, -2.2f, "Yy"};
            dataframe_append_row(df, demo_row);
            extract_row = dataframe_get_row(df, 4);
            REQUIRE( get<char>(extract_row[1]) == 'Y');
            REQUIRE( get<string>(extract_row[4]) == "Yy");
        }

        SECTION( "Appending incorrect length row")
        {
            vector<data_element> demo_row = {-1, 'Z'};
            REQUIRE_THROWS_AS( dataframe_append_row(df, demo_row), std::invalid_argument );
        }

        SECTION( "Appending row with incorrect column types" )
        {
            vector<data_element> demo_row = {-1, 'Z', false, -1.1f, -1};
            REQUIRE_THROWS_AS( dataframe_append_row(df, demo_row), std::invalid_argument );
        }
    }

    SECTION( "Inserting columns" )
    {
        dataframe df = create_demo_dataframe();

        SECTION( "Inserting valid columns" )
        {
            vector<data_element> demo_col = {'Z', 'Y', 'X'};
            dataframe_insert_col(df, 2, demo_col, "Col A");
            vector<data_element> extract_col = dataframe_get_col(df, 2);
            REQUIRE( get<char>(extract_col[0]) == 'Z' );
            REQUIRE( get<char>(extract_col[2]) == 'X' );
            extract_col = dataframe_get_col(df, 5);
            REQUIRE( get<string>(extract_col[0]) == "Aa" );
            REQUIRE( get<string>(extract_col[2]) == "Cc" );
            extract_col = dataframe_get_col(df, 1);
            REQUIRE( get<char>(extract_col[0]) == 'A' );
            REQUIRE( get<char>(extract_col[2]) == 'C' );
        }

        SECTION( "Inserting incorrect length column")
        {
            vector<data_element> demo_col = {'Z', 'Y'};
            REQUIRE_THROWS_AS( dataframe_insert_col(df, 1, demo_col, "Col A"), std::invalid_argument );
        }

        SECTION( "Inserting column with variable types" )
        {
            vector<data_element> demo_col = {'Z', 'Y', 1};
            REQUIRE_THROWS_AS( dataframe_insert_col(df, 1, demo_col, "Col A"), std::invalid_argument );
        }
    }

    SECTION( "Inserting rows" )
    {
        dataframe df = create_demo_dataframe();

        SECTION( "Inserting valid rows" )
        {
            vector<data_element> demo_row = {-1, 'Z', false, -1.1f, "Zz"};
            dataframe_insert_row(df, 1, demo_row);
            vector<data_element> extract_row = dataframe_get_row(df, 1);
            REQUIRE( get<int>(extract_row[0]) == -1 );
            REQUIRE( get<char>(extract_row[1]) == 'Z' );
            extract_row = dataframe_get_row(df, 3);
            REQUIRE( get<int>(extract_row[0]) == 8 );
            REQUIRE( get<char>(extract_row[1]) == 'C' );
            extract_row = dataframe_get_row(df, 0);
            REQUIRE( get<int>(extract_row[0]) == 9 );
            REQUIRE( get<char>(extract_row[1]) == 'A' );
        }

        SECTION( "Inserting incorrect length row" )
        {
            vector<data_element> demo_row = {-1, 'Z'};
            REQUIRE_THROWS_AS( dataframe_insert_row(df, 0, demo_row), std::invalid_argument );
        }

        SECTION( "Inserting row with incorrect column types" )
        {
            vector<data_element> demo_row = {-1, 'Z', false, -1.1f, -1};
            REQUIRE_THROWS_AS( dataframe_insert_row(df, 0, demo_row), std::invalid_argument );
        }
    }

    SECTION( "Updating columns" )
    {
        dataframe df = create_demo_dataframe();

        SECTION( "Updating with valid column data" )
        {
            vector<data_element> demo_col = {9, 8, 7};
            dataframe_update_col(df, 2, demo_col, "Col X");
            vector<data_element> extract_col = dataframe_get_col(df, 2);
            REQUIRE( get<int>(extract_col[0]) == 9 );
            REQUIRE( get<int>(extract_col[1]) == 8 );
            extract_col = dataframe_get_col(df, 3);
            REQUIRE( get<float>(extract_col[0]) == 1.1f );
            REQUIRE( get<float>(extract_col[1]) == 2.2f );
        }

        SECTION( "Updating at invalid column indexes" )
        {
            vector<data_element> demo_col = {9, 8, 7};
            REQUIRE_THROWS_AS( dataframe_update_col(df, -1, demo_col, "Col X"), std::out_of_range );
            REQUIRE_THROWS_AS( dataframe_update_col(df, 5, demo_col, "Col X"), std::out_of_range );
        }

        SECTION( "Updating column with incorrect column types")
        {
            vector<data_element> demo_col = {9, 8, '7'};
            REQUIRE_THROWS_AS( dataframe_update_col(df, 2, demo_col, "Col X"), std::invalid_argument );
        }
    }
}
