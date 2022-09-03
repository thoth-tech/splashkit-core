#ifndef dataframe_h
#define dataframe_h

#include <vector>
#include <variant>

#define DATAFRAME_NULL dataframe_get_null()

namespace splashkit_lib
{
    /**
     * A dataframe_null is a null element that can be represent a
     * missing value in a dataframe
     *
     * @attribute class dataframe_null
     */
    typedef struct _dataframe_null_data *dataframe_null;

    /**
     * Allows null data elements to be printed
     *
     * @param stream            Output stream to print to
     * @param data              Null data element to print
     * @return std::ostream&    Output stream to print to
     */
    std::ostream &operator << (std::ostream &stream, dataframe_null &elem);

    /**
     * Returns an instance of a null element for a dataframe
     *
     * @return dataframe_null   Instance of a null element
     */
    dataframe_null dataframe_get_null();

    /**
     * A data element is an element that can be stored in the cell of a
     * dataframe. Its type must be one of the variant types listed in the
     * signature below.
     */
    typedef std::variant<std::string,int,float,bool,char,dataframe_null> data_element;

    /**
     * Dataframes contain a table of information about a data set. Data
     * from csv or text files can be loaded into a dataframe to perform
     * data analysis with.
     *
     * @attribute class dataframe
     */
    typedef struct _dataframe_data *dataframe;

    /**
     * Creates a new dataframe object
     *
     * @return dataframe   The new dataframe object
     */
    dataframe create_dataframe();

    /**
     * Returns the number of rows in a dataframe.
     *
     * @param df    The dataframe
     * @return int  Number of rows in the dataframe
     */
    int dataframe_num_rows(dataframe &df);

    /**
     * Returns the number of columns in a dataframe.
     *
     * @param df    The dataframe
     * @return int  Number of columns in the dataframe
     */
    int dataframe_num_cols(dataframe &df);

    /**
     * Returns a column from a dataframe.
     *
     * @param df                            The dataframe
     * @param idx                           Index of the column to return
     * @return std::vector<data_element>    A column from the dataframe
     */
    std::vector<data_element> dataframe_get_col(dataframe &df, int idx);

    /**
     * Returns a row from a dataframe.
     *
     * @param df                            The dataframe
     * @param idx                           Index of the row to return
     * @return std::vector<data_element>    A row from the dataframe
     */
    std::vector<data_element> dataframe_get_row(dataframe &df, int idx);

    /**
     * Returns a single element from a dataframe.
     *
     * @param df                The dataframe
     * @param row               The row number of the element
     * @param col               The column number of the element
     * @return data_element     An element from the dataframe
     */
    data_element dataframe_get_cell(dataframe &df, int row, int col);

    /**
     * Adds a column to the end of a dataframe.
     *
     * @param df        The dataframe
     * @param data      The column data to add
     * @param col_name  The name of the column
     */
    void dataframe_append_col(dataframe &df, std::vector<data_element> &data, std::string col_name);

    /**
     * Adds a row to the end of a dataframe.
     *
     * @param df    The dataframe
     * @param data  The row data to add
     */
    void dataframe_append_row(dataframe &df, std::vector<data_element> &data);

    /**
     * Inserts a column into a dataframe at a specified index.
     *
     * @param df        The dataframe
     * @param idx       Index in which the column should be inserted
     * @param data      The column data to insert
     * @param col_name  The name of the column
     */
    void dataframe_insert_col(dataframe &df, int idx, std::vector<data_element> &data, std::string col_name);

    /**
     * Inserts a row into a dataframe at a specified index.
     *
     * @param df    The dataframe
     * @param idx   Index in which the row should be inserted
     * @param data  The row data to insert
     */
    void dataframe_insert_row(dataframe &df, int idx, std::vector<data_element> &data);

    /**
     * Drops a column from a dataframe at a specified index and returns that column.
     *
     * @param df                            The dataframe
     * @param idx                           Index of the column to remove
     * @return std::vector<data_element>    The dropped column
     */
    std::vector<data_element> dataframe_delete_col(dataframe &df, int idx);

    /**
     * Drops a row from a dataframe at a specified index and returns that row.
     *
     * @param df                            The dataframe
     * @param idx                           Index of the row to remove
     * @return std::vector<data_element>    The dropped row
     */
    std::vector<data_element> dataframe_delete_row(dataframe &df, int idx);

    /**
     * Updates the data in a column.
     *
     * @param df        The dataframe
     * @param idx       Index in which the column should be updated
     * @param data      The new column data that is replacing the old data
     * @param col_name  The new name of the column
     */
    void dataframe_update_col(dataframe &df, int idx, std::vector<data_element> &data, std::string col_name);

    /**
     * Allows data elements to be printed
     *
     * @param stream            Output stream to print to
     * @param data              data element to print
     * @return std::ostream&    Output stream to print to
     */
    std::ostream &operator << (std::ostream &stream, data_element &data);

    /**
     * Displays the contents of a dataframe in the console.
     *
     * @param df    The dataframe to display.
     */
    void dataframe_display(dataframe &df);

    /**
     * Displays the contents of a single row in a dataframe
     *
     * @param df    The dataframe
     * @param idx   Index of the row to display
     */
    void dataframe_display_row(dataframe &df, int idx);

    /**
     * Displays the contents of a single column in a dataframe
     *
     * @param df    The dataframe
     * @param idx   Index of the column to display
     */
    void dataframe_display_col(dataframe &df, int idx);

    /**
     * Reads the content of a csv or text file and stores it in a dataframe.
     *
     * @param filepath      The path to the csv or text file.
     * @param sep           The character used to separate elements in the file, by default ','.
     * @param line_break    The character used to separate rows in the file, by default '\\n'.
     * @param header        Whether or not row 0 in the file containers the column names.
     * @return dataframe   The new dataframe containing the file's data.
     */
    dataframe dataframe_read_csv(std::string filepath, char sep = ',', char line_break = '\n', bool header = true);
}

#endif
