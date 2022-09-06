#include <iostream>
#include <stdexcept>

#include "dataframe.h"

namespace splashkit_lib
{
    struct _dataframe_null_data
    {
        public:
            static _dataframe_null_data &get_instance()
            {
                static _dataframe_null_data instance;
                return instance;
            }
            _dataframe_null_data(_dataframe_null_data const&) = delete;
            _dataframe_null_data(_dataframe_null_data&&) = delete;
        private:
            _dataframe_null_data() {}
    };

    /**
     * Returns an instance of a null element that can be used anywhere in a dataframe
     *
     * @return dataframe_null   Instance of a null element
     */
    dataframe_null dataframe_get_null()
    {
        return &_dataframe_null_data::get_instance();
    }

    data_element_type dataframe_get_element_type(data_element &elem)
    {
        return (data_element_type) elem.index();
    }

    struct _dataframe_data
    {
        std::vector<std::vector<data_element>> data;    // Dataframe data, data[i][j] = col i row j
        std::vector<std::string> col_names;             // Name of each of the columns in the dataframe
        std::vector<data_element_type> col_types;       // Data type in each column (index of the type in the data_element variant)
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

    std::string dataframe_get_col_type(dataframe &df, int idx)
    {
        // See data_element type definition for how to extend to new data types
        static std::string type_names[] = {"string", "int", "float", "bool", "char", "null"};
        return type_names[df->col_types[idx]];
    }

    std::vector<std::string> dataframe_get_col_types(dataframe &df)
    {
        std::vector<std::string> col_types;
        for (int i = 0; i < dataframe_num_cols(df); i++)
            col_types.push_back(dataframe_get_col_type(df, i));
        return col_types;
    }

    std::string dataframe_get_col_name(dataframe &df, int idx)
    {
        return df->col_names[idx];
    }

    std::vector<std::string> dataframe_get_col_names(dataframe &df)
    {
        std::vector<std::string> col_names;
        for (int i = 0; i < dataframe_num_cols(df); i++)
            col_names.push_back(dataframe_get_col_name(df, i));
        return col_names;
    }

    /**
     * Raises an out_of_range exception if an invalid column index is requested.
     *
     * @param df    The dataframe
     * @param idx   Index of the column
     */
    inline void dataframe_validate_col_idx(dataframe &df, int idx)
    {
        if (idx < 0 || idx >= dataframe_num_cols(df))
            throw std::out_of_range("column " + std::to_string(idx) + " is out of range");
    }

    /**
     * Raises an out_of_range exception if an invalid row index is requested.
     *
     * @param df    The dataframe
     * @param idx   Index of the row
     */
    inline void dataframe_validate_row_idx(dataframe &df, int idx)
    {
        if (idx < 0 || idx >= dataframe_num_rows(df))
            throw std::out_of_range("row " + std::to_string(idx) + " is out of range");
    }

    std::vector<data_element> dataframe_get_col(dataframe &df, int idx)
    {
        dataframe_validate_col_idx(df, idx);
        return df->data[idx];
    }

    std::vector<data_element> dataframe_get_row(dataframe &df, int idx)
    {
        dataframe_validate_row_idx(df, idx);
        std::vector<data_element> row;
        for (int col = 0; col < dataframe_num_cols(df); col++)
            row.push_back(df->data[col][idx]);
        return row;
    }

    data_element dataframe_get_cell(dataframe &df, int row, int col)
    {
        dataframe_validate_row_idx(df, row);
        dataframe_validate_col_idx(df, col);
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

        // Init column type as null
        data_element_type col_type = DATA_ELEMENT_NULL;

        // Set column type and validate data only has one type (or null)
        for (int row = 0; row < data.size(); row++)
        {
            data_element_type row_type = dataframe_get_element_type(data[row]);
            if (row_type == DATA_ELEMENT_NULL)
                continue;
            else if (col_type == DATA_ELEMENT_NULL)
                col_type = row_type;
            else if (col_type != row_type)
                throw std::invalid_argument("Not all data elements in the inserted column are the same type");
        }

        // Insert
        df->col_names.insert(df->col_names.begin() + idx, col_name);
        df->data.insert(df->data.begin() + idx, data);
        df->col_types.insert(df->col_types.begin() + idx, col_type);
    }

    void dataframe_insert_row(dataframe &df, int idx, std::vector<data_element> &data)
    {
        // Validate row length
        if (data.size() != dataframe_num_cols(df))
            throw std::invalid_argument("Number of columns in the inserted row (" + std::to_string(data.size()) + ") does not match the number of columns in the dataframe (" + std::to_string(dataframe_num_cols(df)) + ")");

        // Validate elements match column types
        if (dataframe_num_rows(df) != 0)
            for (int col = 0; col < dataframe_num_cols(df); col++)
                if (!std::holds_alternative<dataframe_null>(data[col]) && df->col_types[col] != dataframe_get_element_type(data[col]))
                    throw std::invalid_argument("Not all data elements in the inserted row match the type of their respective column");

        // Insert
        for (int col = 0; col < dataframe_num_cols(df); col++)
            df->data[col].insert(df->data[col].begin() + idx, data[col]);
    }

    std::vector<data_element> dataframe_delete_col(dataframe &df, int idx)
    {
        std::vector<data_element> col = dataframe_get_col(df, idx);
        df->data.erase(df->data.begin() + idx);
        df->col_names.erase(df->col_names.begin() + idx);
        df->col_types.erase(df->col_types.begin() + idx);
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
        dataframe_validate_col_idx(df, idx);
        dataframe_insert_col(df, idx, data, col_name); // Insert first to validate new column before old column is deleted
        dataframe_delete_col(df, idx+1);
    }

    void dataframe_update_cell(dataframe &df, int col_idx, int row_idx, data_element &data)
    {
        dataframe_validate_col_idx(df, col_idx);
        dataframe_validate_row_idx(df, row_idx);

        data_element_type element_type = dataframe_get_element_type(data);

        if (element_type == DATA_ELEMENT_NULL || element_type == df->col_types[col_idx])
            df->data[col_idx][row_idx] = data;
        else
            throw std::invalid_argument("Datatype of given element is not the same as datatype of column " + dataframe_get_col_type(df, col_idx));
    }


    std::ostream &operator << (std::ostream &stream, dataframe_null &elem)
    {
        // Print a null data element
        stream << "null";
        return stream;
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
}
