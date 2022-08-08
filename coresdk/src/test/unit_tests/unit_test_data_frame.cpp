#include "catch.hpp"
#include "data_frame.h"
#include <stdexcept>

using namespace splashkit_lib;
using std::vector, std::string, std::get;

data_frame create_demo_data_frame()
{
    data_frame df = create_data_frame();
    vector<data_element> demo_col1 = {9, 4, 8};
    vector<data_element> demo_col2 = {'A', 'B', 'C'};
    vector<data_element> demo_col3 = {true, false, true};
    vector<data_element> demo_col4 = {1.1f, 2.2f, 3.3f};
    vector<data_element> demo_col5 = {"Aa", "Bb", "Cc"};
    append_col(df, demo_col1, "Col 1");
    append_col(df, demo_col2, "Col 2");
    append_col(df, demo_col3, "Col 3");
    append_col(df, demo_col4, "Col 4");
    append_col(df, demo_col5, "Col 5");
    return df;
}

TEST_CASE( "Data Frame", "[data_frame]" )
{
    SECTION( "Creating a data frame" )
    {
        data_frame df = create_data_frame();
        REQUIRE( num_rows(df) == 0 );
        REQUIRE( num_cols(df) == 0 );
    }

    SECTION( "Getting number of rows" )
    {
        data_frame df = create_demo_data_frame();
        REQUIRE( num_rows(df) == 3 );
    }

    SECTION( "Getting number of columns" )
    {
        data_frame df = create_demo_data_frame();
        REQUIRE( num_cols(df) == 5 );
    }

    SECTION( "Getting columns" )
    {
        data_frame df = create_demo_data_frame();

        SECTION( "Getting valid columns")
        {
            vector<data_element> demo_col = get_col(df, 0);
            REQUIRE( get<int>(demo_col[0]) == 9 );
            REQUIRE( get<int>(demo_col[2]) == 8 );

            demo_col = get_col(df, 4);
            REQUIRE( get<string>(demo_col[0]) == "Aa" );
            REQUIRE( get<string>(demo_col[1]) == "Bb" );
        }

        SECTION( "Getting invalid column indexes")
        {
            REQUIRE_THROWS_AS( get_col(df, -1), std::out_of_range );
            REQUIRE_THROWS_AS( get_col(df, 5), std::out_of_range );
        }
    }

    SECTION( "Getting rows" )
    {
        data_frame df = create_demo_data_frame();

        SECTION( "Getting valid rows")
        {
            vector<data_element> demo_row = get_row(df, 0);
            REQUIRE( get<char>(demo_row[1]) == 'A' );
            REQUIRE( get<float>(demo_row[3]) == 1.1f );

            demo_row = get_row(df, 2);
            REQUIRE( get<char>(demo_row[1]) == 'C' );
            REQUIRE( get<float>(demo_row[3]) == 3.3f );
        }

        SECTION( "Getting invalid row indexes")
        {
            REQUIRE_THROWS_AS( get_row(df, -1), std::out_of_range );
            REQUIRE_THROWS_AS( get_row(df, 3), std::out_of_range );
        }
    }

    SECTION( "Getting cells" )
    {
        data_frame df = create_demo_data_frame();

        SECTION( "Getting valid cells" )
        {
            REQUIRE( get<string>(get_cell(df, 1, 4)) == "Bb");
            REQUIRE( get<int>(get_cell(df, 2, 0)) == 8);
        }

        SECTION( "Getting invalid cell indexes" )
        {
            REQUIRE_THROWS_AS( get_cell(df, -1, 1), std::out_of_range );
            REQUIRE_THROWS_AS( get_cell(df, 1, -1), std::out_of_range );
            REQUIRE_THROWS_AS( get_cell(df, 1, 5), std::out_of_range );
            REQUIRE_THROWS_AS( get_cell(df, 3, 1), std::out_of_range );
        }
    }

    SECTION( "Appending columns" )
    {
        data_frame df = create_demo_data_frame();

        SECTION( "Appending valid columns")
        {
            vector<data_element> demo_col = {-1, -2, -3};
            append_col(df, demo_col, "Col 6");
            vector<data_element> extract_col = get_col(df, 5);
            REQUIRE( get<int>(extract_col[0]) == -1 );
            REQUIRE( get<int>(extract_col[2]) == -3 );

            demo_col = {'Z', 'Y', 'X'};
            append_col(df, demo_col, "Col 7");
            extract_col = get_col(df, 6);
            REQUIRE( get<char>(extract_col[0]) == 'Z');
            REQUIRE( get<char>(extract_col[2]) == 'X');
        }

        SECTION( "Appending incorrect length column" )
        {
            vector<data_element> demo_col = {1, 2};
            REQUIRE_THROWS_AS( append_col(df, demo_col, "Col 6"), std::invalid_argument );
        }

        SECTION( "Appending column with variable types" )
        {
            vector<data_element> demo_col = {1, '2', 3};
            REQUIRE_THROWS_AS( append_col(df, demo_col, "Col 6"), std::invalid_argument );
        }
    }

    SECTION( "Appending rows" )
    {
        data_frame df = create_demo_data_frame();

        SECTION( "Appending valid rows" )
        {
            vector<data_element> demo_row = {-1, 'Z', false, -1.1f, "Zz"};
            append_row(df, demo_row);
            vector<data_element> extract_row = get_row(df, 3);
            REQUIRE( get<int>(extract_row[0]) == -1 );
            REQUIRE( get<float>(extract_row[3]) == -1.1f );

            demo_row = {-2, 'Y', true, -2.2f, "Yy"};
            append_row(df, demo_row);
            extract_row = get_row(df, 4);
            REQUIRE( get<char>(extract_row[1]) == 'Y');
            REQUIRE( get<string>(extract_row[4]) == "Yy");
        }

        SECTION( "Appending incorrect length row")
        {
            vector<data_element> demo_row = {-1, 'Z'};
            REQUIRE_THROWS_AS( append_row(df, demo_row), std::invalid_argument );
        }

        SECTION( "Appending row with incorrect column types" )
        {
            vector<data_element> demo_row = {-1, 'Z', false, -1.1f, -1};
            REQUIRE_THROWS_AS( append_row(df, demo_row), std::invalid_argument );
        }
    }

    SECTION( "Inserting columns" )
    {
        data_frame df = create_demo_data_frame();

        SECTION( "Inserting valid columns" )
        {
            vector<data_element> demo_col = {'Z', 'Y', 'X'};
            insert_col(df, 2, demo_col, "Col A");
            vector<data_element> extract_col = get_col(df, 2);
            REQUIRE( get<char>(extract_col[0]) == 'Z' );
            REQUIRE( get<char>(extract_col[2]) == 'X' );
            extract_col = get_col(df, 5);
            REQUIRE( get<string>(extract_col[0]) == "Aa" );
            REQUIRE( get<string>(extract_col[2]) == "Cc" );
            extract_col = get_col(df, 1);
            REQUIRE( get<char>(extract_col[0]) == 'A' );
            REQUIRE( get<char>(extract_col[2]) == 'C' );
        }

        SECTION( "Inserting incorrect length column")
        {
            vector<data_element> demo_col = {'Z', 'Y'};
            REQUIRE_THROWS_AS( insert_col(df, 1, demo_col, "Col A"), std::invalid_argument );
        }

        SECTION( "Inserting column with variable types" )
        {
            vector<data_element> demo_col = {'Z', 'Y', 1};
            REQUIRE_THROWS_AS( insert_col(df, 1, demo_col, "Col A"), std::invalid_argument );
        }
    }

    SECTION( "Inserting rows" )
    {
        data_frame df = create_demo_data_frame();

        SECTION( "Inserting valid rows" )
        {
            vector<data_element> demo_row = {-1, 'Z', false, -1.1f, "Zz"};
            insert_row(df, 1, demo_row);
            vector<data_element> extract_row = get_row(df, 1);
            REQUIRE( get<int>(extract_row[0]) == -1 );
            REQUIRE( get<char>(extract_row[1]) == 'Z' );
            extract_row = get_row(df, 3);
            REQUIRE( get<int>(extract_row[0]) == 8 );
            REQUIRE( get<char>(extract_row[1]) == 'C' );
            extract_row = get_row(df, 0);
            REQUIRE( get<int>(extract_row[0]) == 9 );
            REQUIRE( get<char>(extract_row[1]) == 'A' );
        }

        SECTION( "Inserting incorrect length row" )
        {
            vector<data_element> demo_row = {-1, 'Z'};
            REQUIRE_THROWS_AS( insert_row(df, 0, demo_row), std::invalid_argument );
        }

        SECTION( "Inserting row with incorrect column types" )
        {
            vector<data_element> demo_row = {-1, 'Z', false, -1.1f, -1};
            REQUIRE_THROWS_AS( insert_row(df, 0, demo_row), std::invalid_argument );
        }
    }
}
