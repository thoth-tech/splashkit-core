#ifndef data_frame_h
#define data_frame_h

#include <vector>
#include <variant>

namespace splashkit_lib
{
    /**
     * A data element is an element that can be stored in the cell of a
     * data frame. Its type must be one of the variant types listed in the
     * signature below.
     */
    typedef std::variant<std::string,int,float,bool,char> data_element;

    /**
     * Data frames contain a table of information about a data set. Data
     * from csv or text files can be loaded into a data frame to perform
     * data analysis with.
     *
     * @attribute class data_frame
     */
    typedef struct _data_frame_data *data_frame;

    /**
     * Creates a new data frame object
     *
     * @return data_frame   The new data frame object
     */
    data_frame create_data_frame();

    /**
     * Returns the number of rows in a data frame.
     *
     * @param df    The data frame
     * @return int  Number of rows in the data frame
     */
    int num_rows(data_frame &df);

    /**
     * Returns the number of columns in a data frame.
     *
     * @param df    The data frame
     * @return int  Number of columns in the data frame
     */
    int num_cols(data_frame &df);

    /**
     * Returns a column from a data frame.
     *
     * @param df                            The data frame
     * @param idx                           Index of the column to return
     * @return std::vector<data_element>    A column from the data frame
     */
    std::vector<data_element> get_col(data_frame &df, int idx);

    /**
     * Returns a row from a data frame.
     *
     * @param df                            The data frame
     * @param idx                           Index of the row to return
     * @return std::vector<data_element>    A row from the data frame
     */
    std::vector<data_element> get_row(data_frame &df, int idx);

    /**
     * Returns a single element from a data frame.
     *
     * @param df                The data frame
     * @param row               The row number of the element
     * @param col               The column number of the element
     * @return data_element     An element from the data frame
     */
    data_element get_cell(data_frame &df, int row, int col);

    /**
     * Adds a column to the end of a data frame.
     *
     * @param df        The data frame
     * @param data      The column data to add
     * @param col_name  The name of the column
     */
    void append_col(data_frame &df, std::vector<data_element> &data, std::string col_name);

    /**
     * Adds a row to the end of a data frame.
     *
     * @param df    The data frame
     * @param data  The row data to add
     */
    void append_row(data_frame &df, std::vector<data_element> &data);

    /**
     * Inserts a column into a data frame at a specified index.
     *
     * @param df        The data frame
     * @param idx       Index in which the column should be inserted
     * @param data      The column data to insert
     * @param col_name  The name of the column
     */
    void insert_col(data_frame &df, int idx, std::vector<data_element> &data, std::string col_name);

    /**
     * Inserts a row into a data frame at a specified index.
     *
     * @param df    The data frame
     * @param idx   Index in which the row should be inserted
     * @param data  The row data to insert
     */
    void insert_row(data_frame &df, int idx, std::vector<data_element> &data);

    /**
     * Drops a column from a data frame at a specified index and returns that column.
     *
     * @param df                            The data frame
     * @param idx                           Index of the column to remove
     * @return std::vector<data_element>    The dropped column
     */
    std::vector<data_element> delete_col(data_frame &df, int idx);

    /**
     * Drops a row from a data frame at a specified index and returns that row.
     *
     * @param df                            The data frame
     * @param idx                           Index of the row to remove
     * @return std::vector<data_element>    The dropped row
     */
    std::vector<data_element> delete_row(data_frame &df, int idx);

    /**
     * Allows data elements to be printed
     *
     * @param stream            Output stream to print to
     * @param data              data element to print
     * @return std::ostream&    Output stream to print to
     */
    std::ostream &operator << (std::ostream &stream, data_element &data);

    /**
     * Displays the contents of a data frame in the console.
     *
     * @param df    The data frame to display.
     */
    void display(data_frame &df);

    /**
     * Displays the contents of a single row in a data frame
     *
     * @param df    The data frame
     * @param idx   Index of the row to display
     */
    void display_row(data_frame &df, int idx);

    /**
     * Displays the contents of a single column in a data frame
     *
     * @param df    The data frame
     * @param idx   Index of the column to display
     */
    void display_col(data_frame &df, int idx);

    /**
     * Reads the content of a csv or text file and stores it in a data frame.
     *
     * @param filepath      The path to the csv or text file.
     * @param sep           The character used to separate elements in the file, by default ','.
     * @param line_break    The character used to separate rows in the file, by default '\\n'.
     * @param header        Whether or not row 0 in the file containers the column names.
     * @return data_frame   The new data frame containing the file's data.
     */
    data_frame read_csv(std::string filepath, char sep = ',', char line_break = '\n', bool header = true);

    /**
     * Raises an out_of_range exception if an invalid column index is requested.
     *
     * @param df    The data frame
     * @param idx   Index of the column
     */
    void validate_col(data_frame &df, int idx);

    /**
     * Raises an out_of_range exception if an invalid row index is requested.
     *
     * @param df    The data frame
     * @param idx   Index of the row
     */
    void validate_row(data_frame &df, int idx);
}

#endif
