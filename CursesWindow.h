//
// Interface definition for the CursesWindow Class
// Provides a simple class encapsulating ncurses functionality to display route on grid
// Author: Max Benson
// Date: 08/07/2021
//

#ifndef CURSESWINDOW_H
#define CURSESWINDOW_H

#include <stack>
using std::stack;

#include <curses.h>
#include "Grid.h"

class CursesWindow {
public:
    CursesWindow();
    ~CursesWindow();

    void InitGraphics();

    void ShowGrid(const Grid& grid);
    void ShowPath(stack<GridLocation> path, bool fStayOn);

private:
    void PlotPath(stack<GridLocation> path, bool fOn);
    void PlotBorder(chtype ch, unsigned row, unsigned col );
    void PlotObstacle(unsigned row, unsigned col);
    void PlotCorridor(unsigned row, unsigned col);
    void PlotPath(chtype ch, unsigned row, unsigned col);

    bool     _fInit;
    unsigned _numberRows;
    unsigned _numberColumns;
};

#endif //CURSESWINDOW_H
