//
// Method implementation for the GridLocation Class
// Author: Max Benson
// Date: 08/15/2021
// Modifier: Ilya Bukhalau
// Modification Date: 10/21/22
//

#include <cassert>
#include "GridLocation.h"

/**
 * Default constructor
 * Creates an location of (0,0)
 */
GridLocation::GridLocation() {
    _row = 0;
    _col = 0;

}

/**
 * Constructor
 * Creates a specific location
 * @param row - row number
 * @param col - column number
 */
GridLocation::GridLocation(size_t row, size_t col) {
    _row = row;
    _col = col;
}

/**
 * Returns row number
 * @return row
 */
size_t GridLocation::Row() const {
    return _row;
}

/**
 * Returns column number
 * @return column
 */
size_t GridLocation::Col() const {
    return _col;
}

/**
 * Return a string-ized version of a GridLocation object
 * @return string
 */
string GridLocation::ToString() const {
    return "<"+to_string(_row)+","+to_string(_col)+">";
}

/*
 * Is the lhs < rhs?
 * In order to use STL templated set we need to implement the < operator
 *     Here we're going to define < to mean that either the row number
 *     of lhs is less than the row number of rhs, or the row numbers
 *     are the same and the column number of the lhs is less than the
 *     column number of the rhs
 * @param lhs left hand side of the comparison
 * @param rhs right hand side of the comparison
 * @returns true or false
 */
bool operator <(const GridLocation &lhs, const GridLocation& rhs) {
    if (lhs._row < rhs._row || lhs._col < rhs._col) {
        return true;
    }
    return false;
}

/*
 * Is the lhs == rhs?
 * This allow our maze solver to check if two grid locations are the same
 * @param lhs left hand side of the comparison
 * @param rhs right hand side of the comparison
 * @returns true or false
 */
bool operator ==(const GridLocation &lhs, const GridLocation& rhs) {
    if (lhs._row == rhs._row && lhs._col == rhs._col) {
        return true;
    }
    return false;
}
