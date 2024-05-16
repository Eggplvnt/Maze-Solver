//
// Declaration of maze solution functions
// Author: Max Benson
// Date: 08/16/2021
//

#ifndef MAZE_H
#define MAZE_H

bool SolveMaze(const Grid& maze, stack<GridLocation>& solution, CursesWindow* pwindow = nullptr);
void GenerateValidMoves(const Grid& maze, const GridLocation& loc, GridLocation moves[], size_t& count);
bool CheckSolution(const Grid& maze, stack<GridLocation> path);

#endif //MAZE_H
