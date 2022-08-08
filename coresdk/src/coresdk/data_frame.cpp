#include <iostream>
#include <stdexcept>

#include "data_frame.h"

namespace splashkit_lib
{
    struct _data_frame_data
    {
        std::vector<std::vector<data_element>> data;    // Data frame data, data[i][j] = col i row j
        std::vector<std::string> col_names;             // Name of each of the columns in the data frame
    };

    data_frame create_data_frame()
    {
        data_frame df = new _data_frame_data();
        return df;
    }

    int num_rows(data_frame &df)
    {
        if (num_cols(df) == 0)
            return 0;
        return df->data[0].size();
    }

    int num_cols(data_frame &df)
    {
        return df->col_names.size();
    }

    std::vector<data_element> get_col(data_frame &df, int idx)
    {
        return df->data[idx];
    }

    std::vector<data_element> get_row(data_frame &df, int idx)
    {
        std::vector<data_element> row;
        for (int col = 0; col < num_cols(df); col++)
            row.push_back(df->data[col][idx]);
        return row;
    }

    data_element get_cell(data_frame &df, int row, int col)
    {
        return df->data[col][row];
    }

    void append_col(data_frame &df, std::vector<data_element> &data, std::string col_name)
    {
        insert_col(df, num_cols(df), data, col_name);
    }

    void append_row(data_frame &df, std::vector<data_element> &data)
    {
        insert_row(df, num_rows(df), data);
    }

    void insert_col(data_frame &df, int idx, std::vector<data_element> &data, std::string col_name)
    {
        // Validate column length
        if (num_cols(df) != 0 && data.size() != num_rows(df))
            throw std::runtime_error("Number of rows in the inserted column (" + std::to_string(data.size()) + ") does not match the number of rows in the data frame (" + std::to_string(num_rows(df)) + ")");

        // Validate same type within column
        for (int row = 1; row < data.size(); row++)
            if (data[row].index() != data[row-1].index())
                throw std::runtime_error("Not all data elements in the inserted column are the same type");

        // Insert
        df->col_names.insert(df->col_names.begin() + idx, col_name);
        df->data.insert(df->data.begin() + idx, data);
    }

    void insert_row(data_frame &df, int idx, std::vector<data_element> &data)
    {
        // Validate row length
        if (data.size() != num_cols(df))
            throw std::runtime_error("Number of columns in the inserted row (" + std::to_string(data.size()) + ") does not match the number of columns in the data frame (" + std::to_string(num_cols(df)) + ")");

        // Validate elements match column types
        if (num_rows(df) != 0)
            for (int col = 0; col < num_cols(df); col++)
                if (data[col].index() != df->data[col][0].index())
                    throw std::runtime_error("Not all data elements in the inserted row match the type of their respective column");

        // Insert
        for (int col = 0; col < num_cols(df); col++)
            df->data[col].insert(df->data[col].begin() + idx, data[col]);
    }

    std::vector<data_element> delete_col(data_frame &df, int idx)
    {
        std::vector<data_element> col = get_col(df, idx);
        df->data.erase(df->data.begin() + idx);
        return col;
    }

    std::vector<data_element> delete_row(data_frame &df, int idx)
    {
        std::vector<data_element> row = get_row(df, idx);
        for (int col = 0; col < num_cols(df); col++)
            df->data[col].erase(df->data[col].begin() + idx);
        return row;
    }

    /**
     * Allows data elements to be printed
     *
     * @param stream            Output stream to print to
     * @param data              data element to print
     * @return std::ostream&    Output stream to print to
     */
    std::ostream &operator << (std::ostream &stream, data_element &data)
    {
        std::visit([&stream](auto&& d) { stream << d; }, data);
        return stream;
    }

    void display(data_frame &df)
    {
        // Display column names
        for (std::string col_name : df->col_names)
            std::cout << col_name << '\t';
        std::cout << std::endl;

        // Display data
        for (int row = 0; row < num_rows(df); row++)
            display_row(df, row);
    }

    void display_row(data_frame &df, int idx)
    {
        for (data_element cell : get_row(df, idx))
            std::cout << cell << '\t';
        std::cout << std::endl;
    }

    void display_col(data_frame &df, int idx)
    {
        for (data_element cell : get_col(df, idx))
            std::cout << cell << '\n';
    }

    data_frame read_csv(std::string filepath, char sep, char line_break, bool header)
    {
        // TODO
        data_frame df = new _data_frame_data();
        return df;
    }
}