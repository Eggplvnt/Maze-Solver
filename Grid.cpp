//
// Method implementation for the Grid Class
// Author: Max Benson
// Date: 08/15/2021
// Modifier: <your name>
// Modification Date: <date>
//

#include <cassert>
#include "Grid.h"
using namespace std;

/**
 * Default constructor
 * Creates an 0x0 grid
 */
Grid::Grid() {
    _nRows = 0;
    _nCols = 0;
    //_array[0][0] = {};
}

/**
 * Destructor
 * Frees the dynamic memory allocated for the list
 */
Grid::~Grid() {
    /*for (int row = 0; row < _nRows; row++) {
        for (int col = 0; col < _nCols; col++) {
            delete[] _array;
        }
    }*/
    /*for (int row = 0; row < _nRows; row++) {
        for (int col = 0; col <_nCols; col++) {
            delete[] _array[col];
        }
    }*/
}

/**
 * Configure nRows x nCols grid of bool initially filled with false
 * 1) Frees any existing allocation
 * 2) Allocates storage for the nRows x nCols grid
 * 3) Initializes it to all false
 * @param nRows = number of rows
 * @param nCols = number of columns
 */
void Grid::Configure(size_t nRows, size_t nCols) {
    _nRows = nRows;
    _nCols = nCols;
    /*vector<vector<bool>> here;

    for (int row = 0; row < nRows; row++) {
        vector<bool> size;
        for (int col = 0; col < nCols; col++) {
            size.push_back(false);
            cout << size[0] << " ";
        }
        here.push_back(size);
        cout << endl;
    }
    _grid = &here;*/

    //bool** change[nRows][nCols];

    //_array = reinterpret_cast<bool **>(_array[nRows][nCols]);
    _array = (bool**)malloc(nRows*sizeof(bool*));
        for (int i = 0; i < nRows; i++) {
            _array[i] = (bool*) malloc(nCols*sizeof(bool));
        }

    for (int row = 0; row < nRows; row++) {
        for (int col = 0; col < nCols; col++) {
            _array[row][col] = false;
        }
    }

}

/**
 * Return number of rows in grid
 * @return number of rows
 */
size_t Grid::NumberRows() const {
    return _nRows;
}

/**
 * Return number of columns in grid
 * @return number of columns
 */
size_t Grid::NumberCols() const {
    return _nCols;
}


/**
 * Overload [] operator for indexing grid with a GridLocation (for retrieval)
 * @param loc grid location
 * @return true/false stored at that location in grid
 */
bool Grid::operator[] (const GridLocation& loc) const {
    return (bool) _array[loc.Row()][loc.Col()];
}

/**
 * Overload [] operator for indexing grid with a GridLocation (for update)
 * @param loc grid location
 * @return settable location fo that location on the grid
 */
bool& Grid::operator[] (const GridLocation& loc) {
    return (bool &) _array[loc.Row()][loc.Col()];
}

/**
 * Determine whether a GridLocaiton is within limits of grid
 * @param loc grid location
 * @return true if within grid, false if not
 */
bool Grid::IsWithinGrid(const GridLocation& loc) const {
    if (loc.Row() < _nRows && loc.Row() >= 0 && loc.Col() < _nCols && loc.Col() >= 0) {
        return true;
    }
    return false;
}

/**
 * Load grid with information read from an input file stream
 * @param is file stream to read from
 * @return true if read succesful, false if not
 */
bool Grid::LoadFromFile(istream& is) {
    size_t nRows;
    size_t nCols;

    // Sample Format:
    // 5 7
    // -------
    //-@@@@@-
    //-----@-
    //-@@@-@-
    //-@---@-
    //
    string line;

    // Read from stream
    is >> nRows;
    is >> nCols;
    getline(is, line);  // Discard remainder of line number of columns is on
    if (nRows <= 0 || nCols <= 0) {
        return false;
    }

    // Setup storage
    Configure(nRows, nCols);

    // Read maze and fill with true/false
    for (size_t row = 0; row < NumberRows(); row++) {
        getline(is, line);
        if (line.length() != NumberCols()) {
            //return false;
        }
        for (size_t col = 0; col < NumberCols(); col++) {
            if (line[col] == '@') {
                (*this)[GridLocation(row,col)] = false;
            }
            else if (line[col] == '-') {
                (*this)[GridLocation(row,col)] = true;
            }
            else {
                return false;
            }
        }
    }
    return true;
}
