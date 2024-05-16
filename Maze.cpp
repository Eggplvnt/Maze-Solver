//
// Maze solution
// Author: Max Benson
// Date: 08/16/2021
// Modifier: Ilya Bukhalau
// Modification Date: 10/23/2022
//

#include <cassert>
#include <stack>
#include <queue>
#include <set>
using std::stack;
using std::queue;
using std::set;

#include "Grid.h"
#include "CursesWindow.h"
#include "Maze.h"

/**
* Attempt to solve the maze using a breadth first algorithm
* @param maze the maze that we want to solve
* @param solution out parameter used to return solution if it is found
* @param pwindow if not nullptr, used to animate the solution process
* @return true if solution can be found, false otherwise
*/
bool SolveMaze(const Grid& maze, stack<GridLocation>& solution, CursesWindow* pwindow) {

    GridLocation start = (GridLocation(0, 0));
    if (!maze[start]) {
        return false;
    }

    queue<stack<GridLocation>> paths;
    stack<GridLocation> currentPath;
    currentPath.push(start);
    paths.push(currentPath);

    int cur = 1;
    vector<GridLocation> vec[50];
    vec->push_back(start);

    set<GridLocation> visited;
    visited.insert(start);

    while (!paths.empty()) {
            currentPath = paths.front();
            paths.pop();

            if (pwindow) {
                pwindow->ShowPath(currentPath, false);
            }

            GridLocation moves[4];
            size_t count = 0;
            GenerateValidMoves(maze, currentPath.top(), moves, count);

            for (int i = 0; i < count; i++) {

                stack<GridLocation> newPath;
                newPath = currentPath;

                int found = 0;
                for (int p = 0; p < cur; p++) {
                    if (moves[i] == vec->at(p)) {
                        found = 1;
                    }
                }


                if (found == 0) {

                    newPath.push(moves[i]);
                    vec->push_back(moves[i]);
                    cur++;
                    paths.push(newPath);

                }
                if (newPath.top() == GridLocation(maze.NumberRows() - 1, maze.NumberCols() - 1)) {

                    solution = newPath;
                    return true;

                }
            }
        }
    return false;
}

/**
* Generate set of grid locations adjacent to "loc" that are within the maze and not walls
* @param maze the maze that we want to solve
* @param loc grid location that we want to move from
* @return set of valid moves
*/
void GenerateValidMoves(const Grid& maze, const GridLocation& loc, GridLocation moves[], size_t& count) {
    GridLocation n = (GridLocation(loc.Row() - 1, loc.Col()));
    GridLocation e = (GridLocation(loc.Row(), loc.Col() + 1));
    GridLocation s = (GridLocation(loc.Row() + 1, loc.Col()));
    GridLocation w = (GridLocation(loc.Row(), loc.Col() - 1));
    count = 0;
    if (maze.IsWithinGrid(n)) {
        if (maze[n]) {
            moves[count] = n;
            count++;
        }
    }
    if (maze.IsWithinGrid(e)) {
        if (maze[e]) {
            moves[count] = e;
            count++;
        }
    }
    if (maze.IsWithinGrid(s)) {
        if (maze[s]) {
            moves[count] = s;
            count++;
        }
    }
    if (maze.IsWithinGrid(w)) {
        if (maze[w]) {
            moves[count] = w;
            count++;
        }
    }
}

/**
* Check whether a purported solution is really a solution
* @param maze the maze that we want to solve
* @param path solution to check
* @return true it solves the maze, false it doesn't
*/
bool CheckSolution(const Grid& maze, stack<GridLocation> path) {
    set<GridLocation> old;
    size_t count = 0;
    GridLocation current = (GridLocation(maze.NumberRows() - 1, maze.NumberCols() - 1));
    GridLocation end = (GridLocation(0, 0));
    if (current == path.top()) {
        old.insert(path.top());
        path.pop();
        current = path.top();
        while (!path.empty()) {
            GridLocation valMove[4];
            GenerateValidMoves(maze, current, valMove, count);
            for (int i = 0; i < count; i++) {
                if (maze[current] && maze[valMove[i]]) {
                    if (old.count(current) == 0) {
                        old.insert(path.top());
                        path.pop();
                        if (!path.empty()) {
                            current = path.top();
                        }
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        }
        if (current == end) {
            return true;
        }
    }
    return false;
}

