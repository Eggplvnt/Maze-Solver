//
// Interface Definition for the GridLocation Class
// Author: Max Benson
// Date: 08/15/2021
//

#ifndef GRIDLOCATION_H
#define GRIDLOCATION_H

#include <string>
using std::string;
using std::to_string;

class GridLocation {
public:
    GridLocation();
    GridLocation(size_t row, size_t col);

    size_t Row() const;
    size_t Col() const;

    string ToString() const;

    friend bool operator <(const GridLocation &lhs, const GridLocation& rhs) ;
    friend bool operator==(const GridLocation& lhs, const GridLocation& rhs);

private:
    size_t _row;
    size_t _col;
};


#endif //GRIDLOCATION_H
