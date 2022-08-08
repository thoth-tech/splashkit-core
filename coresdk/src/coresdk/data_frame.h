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

    data_frame create_data_frame();

    int num_rows(data_frame &df);

    int num_cols(data_frame &df);

    std::vector<data_element> get_col(data_frame &df, int idx);

    std::vector<data_element> get_row(data_frame &df, int idx);

    data_element get_cell(data_frame &df, int row, int col);

    void append_col(data_frame &df, std::vector<data_element> &data, std::string col_name);

    void append_row(data_frame &df, std::vector<data_element> &data);

    void insert_col(data_frame &df, int idx, std::vector<data_element> &data, std::string col_name);

    void insert_row(data_frame &df, int idx, std::vector<data_element> &data);

    std::vector<data_element> delete_col(data_frame &df, int idx);

    std::vector<data_element> delete_row(data_frame &df, int idx);

    /**
     * Displays the contents of a data frame in the console.
     *
     * @param df    The data frame to display.
     */
    void display(data_frame &df);

    void display_row(data_frame &df, int idx);

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
}

#endif