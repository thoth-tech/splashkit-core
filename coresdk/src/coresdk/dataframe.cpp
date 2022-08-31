#include <iostream>
#include <stdexcept>

#include "dataframe.h"

namespace splashkit_lib
{
    struct _dataframe_data
    {
        std::vector<std::vector<data_element>> data;    // Dataframe data, data[i][j] = col i row j
        std::vector<std::string> col_names;             // Name of each of the columns in the dataframe
    };

    dataframe create_dataframe()
    {
        dataframe df = new _dataframe_data();
        return df;
    }

    int dataframe_num_rows(dataframe &df)
    {
        if (dataframe_num_cols(df) == 0)
            return 0;
        return df->data[0].size();
    }

    int dataframe_num_cols(dataframe &df)
    {
        return df->col_names.size();
    }

    std::vector<data_element> dataframe_get_col(dataframe &df, int idx)
    {
        dataframe_validate_col(df, idx);
        return df->data[idx];
    }

    std::vector<data_element> dataframe_get_row(dataframe &df, int idx)
    {
        dataframe_validate_row(df, idx);
        std::vector<data_element> row;
        for (int col = 0; col < dataframe_num_cols(df); col++)
            row.push_back(df->data[col][idx]);
        return row;
    }

    data_element dataframe_get_cell(dataframe &df, int row, int col)
    {
        dataframe_validate_row(df, row);
        dataframe_validate_col(df, col);
        return df->data[col][row];
    }

    void dataframe_append_col(dataframe &df, std::vector<data_element> &data, std::string col_name)
    {
        dataframe_insert_col(df, dataframe_num_cols(df), data, col_name);
    }

    void dataframe_append_row(dataframe &df, std::vector<data_element> &data)
    {
        dataframe_insert_row(df, dataframe_num_rows(df), data);
    }

    void dataframe_insert_col(dataframe &df, int idx, std::vector<data_element> &data, std::string col_name)
    {
        // Validate column length
        if (dataframe_num_cols(df) != 0 && data.size() != dataframe_num_rows(df))
            throw std::invalid_argument("Number of rows in the inserted column (" + std::to_string(data.size()) + ") does not match the number of rows in the dataframe (" + std::to_string(dataframe_num_rows(df)) + ")");

        // Validate same type within column
        for (int row = 1; row < data.size(); row++)
            if (data[row].index() != data[row-1].index())
                throw std::invalid_argument("Not all data elements in the inserted column are the same type");

        // Insert
        df->col_names.insert(df->col_names.begin() + idx, col_name);
        df->data.insert(df->data.begin() + idx, data);
    }

    void dataframe_insert_row(dataframe &df, int idx, std::vector<data_element> &data)
    {
        // Validate row length
        if (data.size() != dataframe_num_cols(df))
            throw std::invalid_argument("Number of columns in the inserted row (" + std::to_string(data.size()) + ") does not match the number of columns in the dataframe (" + std::to_string(dataframe_num_cols(df)) + ")");

        // Validate elements match column types
        if (dataframe_num_rows(df) != 0)
            for (int col = 0; col < dataframe_num_cols(df); col++)
                if (data[col].index() != df->data[col][0].index())
                    throw std::invalid_argument("Not all data elements in the inserted row match the type of their respective column");

        // Insert
        for (int col = 0; col < dataframe_num_cols(df); col++)
            df->data[col].insert(df->data[col].begin() + idx, data[col]);
    }

    std::vector<data_element> dataframe_delete_col(dataframe &df, int idx)
    {
        std::vector<data_element> col = dataframe_get_col(df, idx);
        df->data.erase(df->data.begin() + idx);
        return col;
    }

    std::vector<data_element> dataframe_delete_row(dataframe &df, int idx)
    {
        std::vector<data_element> row = dataframe_get_row(df, idx);
        for (int col = 0; col < dataframe_num_cols(df); col++)
            df->data[col].erase(df->data[col].begin() + idx);
        return row;
    }

    void dataframe_update_col(dataframe &df, int idx, std::vector<data_element> &data, std::string col_name)
    {
        dataframe_validate_col(df, idx);
        dataframe_insert_col(df, idx, data, col_name); // Insert first to validate new column before old column is deleted
        dataframe_delete_col(df, idx+1);
    }

    std::ostream &operator << (std::ostream &stream, data_element &data)
    {
        // Print underlying value of a data_element
        std::visit([&stream](auto&& d) { stream << d; }, data);
        return stream;
    }

    void dataframe_display(dataframe &df)
    {
        // Display column names
        for (std::string col_name : df->col_names)
            std::cout << col_name << '\t';
        std::cout << std::endl;

        // Display data
        for (int row = 0; row < dataframe_num_rows(df); row++)
            dataframe_display_row(df, row);
    }

    void dataframe_display_row(dataframe &df, int idx)
    {
        for (data_element cell : dataframe_get_row(df, idx))
            std::cout << cell << '\t';
        std::cout << std::endl;
    }

    void dataframe_display_col(dataframe &df, int idx)
    {
        for (data_element cell : dataframe_get_col(df, idx))
            std::cout << cell << '\n';
    }

    dataframe dataframe_read_csv(std::string filepath, char sep, char line_break, bool header)
    {
        // TODO
        dataframe df = new _dataframe_data();
        return df;
    }

    void dataframe_validate_col(dataframe &df, int idx)
    {
        if (idx < 0 || idx >= dataframe_num_cols(df))
            throw std::out_of_range("column " + std::to_string(idx) + " is out of range");
    }

    void dataframe_validate_row(dataframe &df, int idx)
    {
        if (idx < 0 || idx >= dataframe_num_rows(df))
            throw std::out_of_range("row " + std::to_string(idx) + " is out of range");
    }
}
