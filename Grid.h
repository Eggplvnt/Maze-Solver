//
// Interface Definition for the Grid Class
// Author: Max Benson
// Date: 08/15/2021
// Modifier: Ilya Bukhalau
// Modification Date: 10/20/2022
//

#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <vector>
using std::ostream;
using std::istream;
using std::string;

using std::endl;
using namespace std;

#include "GridLocation.h"

class Grid {
public:
    Grid();
    ~Grid();

    void Configure(size_t nRows, size_t nCols);

    size_t NumberRows() const;
    size_t NumberCols() const;
    bool IsWithinGrid(const GridLocation& loc) const;

    bool operator[] (const GridLocation& loc) const;
    bool& operator[] (const GridLocation& loc);
    bool LoadFromFile(istream& is);

    friend ostream& operator<<(ostream& os, const Grid& grid) {
        for (size_t row = 0; row < grid.NumberRows(); row ++) {
            for (size_t col = 0; col < grid.NumberCols(); col ++) {
                if (grid[GridLocation(row,col)]) {
                    os << '-';
                }
                else {
                    os << '@';
                }
            }
            os << endl;
        }
        return os;
    }

private:
    // Declared private since not needed
    Grid(const Grid& other);
    const Grid& operator=(const Grid& other);

    // Declare your data structure here
    bool** _array;

    size_t _nRows;
    size_t _nCols;

};

#endif //GRID_H
