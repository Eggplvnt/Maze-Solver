//
// Method implementation for the CursesWindow Class
// Provides a simple class encapsulating ncurses functionality to display route on grid
// Author: Max Benson
// Date: 08/07/2021
//

#include <cassert>
#include <chrono>
#include <thread>

#include "CursesWindow.h"

CursesWindow::CursesWindow()  {
    _fInit = false;
}

CursesWindow::~CursesWindow() {
    if (_fInit) {
        endwin();
    }
}

void CursesWindow::InitGraphics() {
    assert(!_fInit);
    initscr();
    getmaxyx(stdscr,_numberRows,_numberColumns);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    init_pair(3, COLOR_WHITE, COLOR_BLUE);
    curs_set(0);
    _fInit = true;
}

void CursesWindow::ShowGrid(const Grid& grid) {
    assert(_fInit);
    PlotBorder(ACS_ULCORNER, 0, 0);
    for (size_t col = 0; col < grid.NumberCols(); col++) {
        PlotBorder(ACS_HLINE, 0, col + 1);
    }
    PlotBorder(ACS_URCORNER, 0, grid.NumberCols()+1);
    for (size_t row = 0; row < grid.NumberRows(); row ++) {
        PlotBorder(ACS_VLINE, row+1, 0);
        for (size_t col = 0; col < grid.NumberCols(); col++) {
            GridLocation loc(row, col);

            if (grid[loc]) {
                PlotCorridor(row+1, col+1);
            }
            else {
                PlotObstacle(row+1, col+1);
            }
        }
        PlotBorder(ACS_VLINE, row+1, grid.NumberCols()+1);
    }
    PlotBorder(ACS_LLCORNER, grid.NumberRows()+1, 0);
    for (size_t col = 0; col < grid.NumberCols(); col++) {
        PlotBorder(ACS_HLINE, grid.NumberRows()+1, col + 1);
    }
    PlotBorder(ACS_LRCORNER, grid.NumberRows()+1, grid.NumberCols()+1);
}

void CursesWindow::ShowPath(stack<GridLocation> path, bool fStayOn) {
    PlotPath(path, true);
    if (!fStayOn) {
        refresh();
        std::chrono::milliseconds dura( 250);
        std::this_thread::sleep_for( dura );
        PlotPath(path, false);
    }
    refresh();
}

void CursesWindow::PlotPath(stack<GridLocation> path, bool fOn) {
    while (!path.empty()) {
        GridLocation loc;

        loc = path.top();
        path.pop();
        if (fOn) {
            PlotPath('*', loc.Row()+1, loc.Col()+1);
        }
        else {
            PlotCorridor(loc.Row()+1, loc.Col()+1);
        }
    }
}

void CursesWindow::PlotObstacle(unsigned row, unsigned col) {
    assert(_fInit);
    attron(COLOR_PAIR(1));
    mvaddch(row, col, ' ');
    attroff(COLOR_PAIR(1));
}

void CursesWindow::PlotCorridor(unsigned row, unsigned col) {
    assert(_fInit);
    attron(COLOR_PAIR(2));
    mvaddch(row, col, ' ');
    attroff(COLOR_PAIR(2));
}

void CursesWindow::PlotPath(chtype ch, unsigned row, unsigned col) {
    assert(_fInit);
    attron(COLOR_PAIR(3));
    mvaddch(row, col,ch);
    attroff(COLOR_PAIR(3));
}

void CursesWindow::PlotBorder(chtype ch, unsigned row, unsigned col) {
    assert(_fInit);
    attron(COLOR_PAIR(1));
    mvaddch(row, col, ch);
    attroff(COLOR_PAIR(1));
}
