#pragma once
//C++ libraries
#include <iostream>
#include <iomanip>
#include <vector>

//Header files
#include "Palette.hpp"
#include "Exception.hpp"

/**
 * Class for printing table
 */
class CTable {
    std::vector< std::vector<std::string>  > VTable;    /**< 2D vector represents table */
    std::vector< std::string > VRow;                    /**< Vector of rows */
    std::vector<size_t> VLen;                           /**< Vector of max lenght for each column */
    const int AddSpaces = 2;                            /**< Default number of spaces in the begining and in the end of row */
    std::string color;                                  /**< Color of print */
    size_t colNum;                                     /**< Number of columns */

    /**
     * Method saves max lenght of word in one column
     * @param text Text
     */
    void UpdateMaxLen( const std::string &text );

    /**
     * Prints first row
     * @param columns Number of columns
     */
    void PrintHeader( int columns ) const;

public:

    /**
     * Constructor
     * @param colNum Number of columns
     * @param color Color of printing
     */
    CTable( int colNum, const std::string &color );

    /**
     * Method for adding header
     * @param header Header
     */
    void AddHeader( const std::string &header );

    /**
     * Method for adding elements into table in turn
     * @param text Text
     */
    void AddToRow( const std::string &text );

    /**
     * Prints the whole table
     */
    void Print() const;

    /**
     * Prints row of dots as underline
     */
    void PrintDots() const;

    /**
     * Getter for size of table
     * @return size
     */
    int GetTableSize() const { return VTable.size(); };
};