#ifndef data_analytics_hpp
#define data_analytics_hpp

#include <string>
#include <vector>
#include <sstream>
using namespace std;
using std::string;

#endif /* data_analytics_h */
namespace splashkit_lib
{
    // string set_decimal_places(string input);

    // string get_file_name(string original_file_path);

    void save_file(string input, string path);

    // void save_file_as(string input);

    bool input_has_symbol(string input);

    // bool input_has_char(string input);

    bool input_is_valid_url(string input);

    // bool input_is_empty(string input);

    bool input_is_email(string input);

    bool input_is_phone_number(string &input);

    bool input_is_time(string input);

    void text_validation(string input);

    // DataFrames
    /*
    DataFrame stores tables of data which can be used by other modules.
    Can store information to be manipulated, selected, sorted, exported
    Has 6 constructors and cannot be created with an Empty Constructor
    Currently Does not accept Char
    */

    template <typename T>
    class DataFrame
    {
    public:
        enum flags
        {
            OVERRIDE_FILE = 0x01,
            APPEND_FILE = 0x02,
            ASCENDING = 0x04,
            DESCENDING = 0x08
        };
        // Basic Constructor/Destructor
        DataFrame() = delete;
        ~DataFrame();

        // Takes DataFrame as R-Value (Allows std::move)
        DataFrame(const DataFrame &&_other);
        // Takes DataFrame as L-Value (Allows copy)
        DataFrame(DataFrame &_other);
        // Import From TextFile
        DataFrame(string _textFileName, bool _index);
        // Create using Array or Vector
        DataFrame(unsigned int _columns, const string (&_tempHeader)[], bool _index);
        DataFrame(unsigned int _columns, const string(_tempHeader)[], bool _index);
        DataFrame(vector<string> _tempHeader, bool _index);

        // Export To FILE
        const void exportToFile(string _fileName, flags _eType = OVERRIDE_FILE) const;

        // Push back new row into dataframe
        void push_back(const string (&_arr)[], unsigned int _length);
        void push_back(const string(_arr)[], unsigned int _length);
        void push_back(const DataFrame<T> _dFrame);

        // Insert new row into DataFrame
        void insert(const string (&_arr)[], unsigned int _length, unsigned int _pos);
        void insert(const string(_arr)[], unsigned int _length, unsigned int _pos);
        void insert(const DataFrame<T> _dFrame, unsigned int _pos);

        // Insert Column

        // Insert with Default Number
        void insertC(const string headerName, string defaultValue = "0");
        // void insertC(const string headerName, const string (&_arr)[]);
        // void insertC(const string headerName, const string (_arr)[]);

        // Delete Row at index
        void deleteRow(unsigned int _row);
        void deleteColumn(unsigned int _column);

        // Indexing Function
        // Return Row
        const DataFrame<T> iLoc(int _row) const { return getRow(_row); };

        // Return Cell
        T &iLoc(unsigned int _row, unsigned int _column) { return m_Table[(_row + (_column * m_Columns))]; };
        const T iLoc(unsigned int _row, unsigned int _column) const { return m_Table[(_row + (_column * m_Columns))]; };

        // Return Column
        const DataFrame<T> Loc(string _column) const { return getColumn(_column); };

        // Get Row
        const DataFrame<T> getRow(unsigned int _row) const;

        // Get Column
        const DataFrame<T> getColumn(unsigned int _column) const;
        const DataFrame<T> getColumn(string _column) const;

        // Possibly Use >> to signify Range. Create Function Capability using members

        // Swap
        void swap(unsigned int _index1, unsigned int _index2);

        // Sort
        void sort(int _column, flags _flag);

        // Moving Function
        void transpose();

        // Display Dataframe
        const void display() const;
        const void fullDisplay() const;

        // ResetIndex
        void resetIndex();

        // Size, Dimensions and ReSize
        const unsigned int size() const { return m_Columns * m_Rows; };
        const string dimensions() const
        {
            std::ostringstream os;
            os << m_Rows << " X " << m_Columns;
            return os.str();
        };
        const void reserve(unsigned int _amount) const { m_Table.reserve(_amount); };

    private:
        unsigned int m_Columns = 0;
        unsigned int m_Rows = 0;
        bool m_Index = true;
        /*Storing as a single table
        By Storing in a single table the memory doesn't fragment,
        the process runs faster but also requires more care especially when adding columns
        This can be optimised in the future */
        vector<string> m_HeaderNames;
        vector<T> m_Table;
        vector<string> m_IndexNames;
        int sMod(int a, int b) { return (a % b + b) % b; };
        double partition(int _low, int _high, int _column, flags _flag);
        void quickSort(int _low, int _high, int _column, flags _flag);
    };
}