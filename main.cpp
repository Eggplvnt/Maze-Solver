//
// Maze solver main program
// Author: Max Benson
// Last Update: 08/17/2022
//
#include <cassert>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stack>
#include <dirent.h>
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::stack;
using std::cout;
using std::endl;
using std::left;
using std::setw;

#include "Grid.h"
#include "CursesWindow.h"
#include "Maze.h"

// Forward declarations of test functions
void TestGridLocationClass(unsigned& testsPassed, unsigned& testsFailed);
void TestGridClass(unsigned& testsPassed, unsigned& testsFailed);
void TestGenerateValidMoves(unsigned& testsPassed, unsigned& testsFailed);
void TestSolve(unsigned& testsPassed, unsigned& testsFailed);
void TestCheckSolution(unsigned& testsPassed, unsigned& testsFailed);
void Test(bool condition, const char* message, unsigned& testsPassed, unsigned& testsFailed);
void DoSolve(string fileName, bool fVisualize);


int main(int argc, char* argv[]) {
    // Process arguments
    if (argc == 2 && strcmp(argv[1], "--test:location") == 0) {
        unsigned testsPassed = 0;
        unsigned testsFailed = 0;

        TestGridLocationClass(testsPassed, testsFailed);

        cout << endl << "TEST SUMMARY: " << 100. * testsPassed / (testsPassed + testsFailed) << "%, PASS = "
             << testsPassed << ", FAIL = " << testsFailed << endl;

        return 0;
    }
    else if (argc == 2 && strcmp(argv[1], "--test:grid") == 0) {
        unsigned testsPassed = 0;
        unsigned testsFailed = 0;

        TestGridClass(testsPassed, testsFailed);

        cout << endl << "TEST SUMMARY: " << 100. * testsPassed / (testsPassed + testsFailed) << "%, PASS = "
             << testsPassed << ", FAIL = " << testsFailed << endl;

        cout << endl;
        cout << "=================================================" << endl;
        cout << "==> Don't forget to run this test under valgrind!" << endl;
        cout << "=================================================" << endl;
        return 0;
    }
    else if (argc == 2 && strcmp(argv[1], "--test:moves") == 0) {
        unsigned testsPassed = 0;
        unsigned testsFailed = 0;

        TestGenerateValidMoves(testsPassed, testsFailed);

        cout << endl << "TEST SUMMARY: " << 100. * testsPassed / (testsPassed + testsFailed) << "%, PASS = "
             << testsPassed << ", FAIL = " << testsFailed << endl;

        return 0;
    }
    else if (argc == 2 && strcmp(argv[1], "--test:checksolution") == 0) {
        unsigned testsPassed = 0;
        unsigned testsFailed = 0;

        TestCheckSolution(testsPassed, testsFailed);

        cout << endl << "TEST SUMMARY: " << 100. * testsPassed / (testsPassed + testsFailed) << "%, PASS = "
             << testsPassed << ", FAIL = " << testsFailed << endl;

        return 0;
    }
    else if (argc == 2 && strcmp(argv[1], "--test:solve") == 0) {
        unsigned testsPassed = 0;
        unsigned testsFailed = 0;

        TestSolve(testsPassed, testsFailed);

        cout << endl << "TEST SUMMARY: " << 100. * testsPassed / (testsPassed + testsFailed) << "%, PASS = "
             << testsPassed << ", FAIL = " << testsFailed << endl;

        return 0;
    }
    else {
        if (argc == 2) {
            DoSolve(argv[1], false);
            return 0;
        }
        else if (argc == 3 && strcmp(argv[1], "--visualize") == 0) {
            DoSolve(argv[2], true);
            return 0;
        }
    }
    cout << "Ways to run:" << '\n';
    cout << "MazeSolver --test:location" << '\n';
    cout << "MazeSolver --test:grid" << '\n';
    cout << "MazeSolver --test:moves" << '\n';
    cout << "MazeSolver --test:checksolution" << '\n';
    cout << "MazeSolver --test:solve" << "\n";
    cerr << "MazeSolver [--visualize] <filename>" << "\n";
    return 1;
}

/**  Tries to solve a maze
 * @param fileName  pathname of maze file
 * @param fVisualize whether to graphically display maze and its solution
 */
void DoSolve(string fileName, bool fVisualize) {
    Grid maze;
    stack<GridLocation> solution;
    CursesWindow window;
    ifstream ifs;

    // Load maze file
    ifs.open(fileName, ifstream::in);
    if (!ifs.good()) {
        cerr << "Can't open '" << fileName << "'" << endl;
        exit(2);
    }
    if (!maze.LoadFromFile(ifs)) {
        cerr << "Load from '" << fileName << "' failed " << endl;
        exit(3);
    }

    // Display maze and solve
    if (fVisualize) {
        window.InitGraphics();
        window.ShowGrid(maze);
        if (SolveMaze(maze, solution, &window)) {
            window.ShowPath(solution, true);
        }
        getch();
    }
    else {
        cerr << "Maze:" << endl;
        cerr << maze;
        if (SolveMaze(maze, solution)) {
            bool correct;
            bool firstItem;
            string s;

            correct = CheckSolution(maze, solution);
            cerr << "Solution:" << endl;
            s += "]";
            firstItem = true;
            while (!solution.empty()) {
                if (!firstItem) {
                    s = "," + s;
                } else {
                    firstItem = false;
                }
                s = solution.top().ToString() + s;
                solution.pop();
            }
            s = "[" + s;
            cout << s << endl;
            if (correct) {
                cerr << "Solution is correct" << endl;
            }
            else {
                cerr << "Solution is not correct" << endl;
            }
        }
        else {
            cerr << "Couldn't find solution to maze." << endl;
        }
    }
    ifs.close();
}

/**  Tests solving one maze
 * @param fileName  pathname of maze file
 * @param fSolvable whether the maze file is solvable
 * @param testsPassed running total of number of tests passed, updated upon return
 * @param testsFailed running total of number of tests failed, updated upon return
 */
void TestMaze(const string& fileName, bool fSolvable, unsigned& testsPassed, unsigned& testsFailed) {
    ifstream ifs;
    stack<GridLocation> solution;
    Grid maze;
    string message;
    bool success = true;

    // Load maze file
    ifs.open(fileName, ifstream::in);
    if (!ifs.good()) {
        message = "Can't open '" + fileName + "'";
        Test(false, message.c_str(), testsPassed, testsFailed  );
        success = false;
    }
    else if (!maze.LoadFromFile(ifs)) {
        message = "Load from '" + fileName + "' failed";
        Test(false, message.c_str(), testsPassed, testsFailed  );
        success = false;
    }
    ifs.close();

    // Display maze and solve
    if (success) {
        if (SolveMaze(maze, solution, nullptr)) {
            bool correct;

            if (!fSolvable) {
                message = "Maze '" + fileName + "' is unsolvable, but solution found";
                Test(false, message.c_str(), testsPassed, testsFailed  );
            }
            else {
                correct = CheckSolution(maze, solution);
                if (correct) {
                    message = "Maze '" + fileName + "' correctly solved";
                    Test(true, message.c_str(), testsPassed, testsFailed);
                }
                else {
                    message = "Incorrect solution to  maze '" + fileName + "'";
                    Test(false, message.c_str(), testsPassed, testsFailed);
                }
            }
        }
        else if (fSolvable) {
            message = "Solution not found to  maze '" + fileName + "'";
            Test(false, message.c_str(), testsPassed, testsFailed);
        }
        else {
            message = "As expected, solution not found to maze '" + fileName + "'";
            Test(true, message.c_str(), testsPassed, testsFailed);
        }
    }
}

/**
 * Performs tests on the SolveMaze function, verifying solutions with CheckSolution
 * @param testsPassed running total of number of tests passed, updated upon return
 * @param testsFailed running total of number of tests failed, updated upon return
 */
void TestSolve(unsigned& testsPassed, unsigned& testsFailed) {
    DIR* dirp;
    struct dirent * dp;
    string directoryName;

    // Check the solvable mazes
    directoryName = "../solvable/";
    dirp = opendir(directoryName.c_str());
    while ((dp = readdir(dirp)) != NULL) {
        if (strlen(dp->d_name) >= 5 && strcmp(&dp->d_name[strlen(dp->d_name)-5], ".maze") == 0) {
            string fileName = directoryName + dp->d_name;

            TestMaze(fileName,true,testsPassed, testsFailed);
        }
    }
    closedir(dirp);

    // Check the unsolvable mazes
    directoryName = "../unsolvable/";
    dirp = opendir(directoryName.c_str());
    while ((dp = readdir(dirp)) != NULL) {
        if (strlen(dp->d_name) >= 5 && strcmp(&dp->d_name[strlen(dp->d_name)-5], ".maze") == 0) {
            string fileName = directoryName + dp->d_name;

            TestMaze(fileName,false,testsPassed, testsFailed);
        }
    }
    closedir(dirp);
}


/**
 * Performs tests on the CheckSolution function
 * @param testsPassed running total of number of tests passed, updated upon return
 * @param testsFailed running total of number of tests failed, updated upon return
 */
void TestCheckSolution(unsigned& testsPassed, unsigned& testsFailed) {
    ifstream ifs;
    Grid maze1;
    Grid maze2;

    ifs.open("../solvable/5x7.maze", ifstream::in);
    if (!ifs.good()) {
        cout << "ATTENTION: Cannot perform testing because cannot open '../solvable/5x7.maze'" << endl;
        exit(2);
    }
    if (!maze1.LoadFromFile(ifs)) {
        cout << "ATTENTION: Cannot perform testing because cannot load '../solvable/5x7.maze'" << endl;
        exit(3);
    }
    ifs.close();

    // Valid solution
    stack<GridLocation> path1;
    for (size_t col = 0; col < 7; col ++) {
        path1.push(GridLocation(0, col));
    }
    for (size_t row = 1; row < 5; row ++) {
        path1.push(GridLocation(row, 6));
    }
    Test(CheckSolution(maze1, path1), "Test valid path", testsPassed, testsFailed  );

    // Doesn't start at upper left corner
    stack<GridLocation> path2;
    for (size_t col = 1; col < 7; col ++) {
        path2.push(GridLocation(0, col));
    }
    for (size_t row = 1; row < 5; row ++) {
        path2.push(GridLocation(row, 6));
    }
    Test(!CheckSolution(maze1, path2), "Test doesn't start at upper left corner", testsPassed, testsFailed  );

    // Doesn't end at lower right corner
    stack<GridLocation> path3;
    for (size_t col = 0; col < 7; col ++) {
        path3.push(GridLocation(0, col));
    }
    for (size_t row = 1; row < 4; row ++) {
        path3.push(GridLocation(row, 6));
    }
    Test(!CheckSolution(maze1, path3), "Test doesn't end at lower right corner", testsPassed, testsFailed  );

    // Doesn't doesn't connect
    stack<GridLocation> path4;
    for (size_t col = 0; col < 6; col ++) {
        path4.push(GridLocation(0, col));
    }
    for (size_t row = 1; row < 5; row ++) {
        path4.push(GridLocation(row, 6));
    }
    Test(!CheckSolution(maze1, path4), "Test doesn't connect", testsPassed, testsFailed  );

    // Goes through a wall
    stack<GridLocation> path5;
    for (size_t row = 0; row < 5; row ++) {
        path5.push(GridLocation(row, 0));
    }
    for (size_t col = 1; col < 7; col ++) {
        path5.push(GridLocation(4, col));
    }
    Test(!CheckSolution(maze1, path5), "Test goes through wall", testsPassed, testsFailed  );

    // Goes outside grid
    stack<GridLocation> path6;
    for (size_t col = 0; col < 8; col ++) {
        path6.push(GridLocation(0, col));
    }
    path6.push(GridLocation(1, 7));
    path6.push(GridLocation(1, 6));
    for (size_t row = 2; row < 5; row ++) {
        path6.push(GridLocation(row, 6));
    }
    Test(!CheckSolution(maze1, path6), "Test goes outside grid", testsPassed, testsFailed  );

    // Load a new maze
    ifs.open("../solvable/4x4.maze", ifstream::in);
    if (!ifs.good()) {
        cout << "ATTENTION: Cannot complete testing because cannot open '../solvable/4x4.maze'" << endl;
        exit(2);
    }
    if (!maze2.LoadFromFile(ifs)) {
        cout << "ATTENTION: Cannot perform testing because cannot load '../solvable/5x7.maze'" << endl;
        exit(3);
    }

    // Loops
    stack<GridLocation> path7;
    for (size_t row = 0; row < 4; row ++) {
        path7.push(GridLocation(row, 0));
    }
    path7.push(GridLocation(3,1));
    path7.push(GridLocation(2,1));
    path7.push(GridLocation(2,0));
    for (size_t col = 0; col < 4; col ++) {
        path7.push(GridLocation(3, col));
    }
    Test(!CheckSolution(maze2, path7), "Test looping path", testsPassed, testsFailed  );

    // End of path is a wall
    Grid maze3;
    stack<GridLocation> path8;

    maze3.Configure(2,2);
    maze3[GridLocation(0,0)] = true;
    maze3[GridLocation(0,1)] = true;
    maze3[GridLocation(1,0)] = true;
    path8.push(GridLocation(0,0));
    path8.push(GridLocation(0,1));
    path8.push(GridLocation(1,1));
    Test(!CheckSolution(maze3, path8), "Test lower right corner is wall", testsPassed, testsFailed  );

    // Start of path is a wall
    maze3[GridLocation(0,0)] = false;
    maze3[GridLocation(0,1)] = true;
    maze3[GridLocation(1,0)] = true;
    maze3[GridLocation(1,1)] = true;
    Test(!CheckSolution(maze3, path8), "Test upper left corner is wall", testsPassed, testsFailed  );
}

/**
 * Loops through the move array to find the GridLocation
 * @param moves array of moves
 * @param count number of moves in the array
 * @param loc GridLocation that should be in the array
 * @return true if found, false if not found
 */
bool FindMove(const GridLocation moves[], size_t count, const GridLocation& loc) {
    for (size_t i = 0; i < count; i ++) {
        if (moves[i] == loc) {
            return true;
        }
    }
    return false;
}

/**
 * Check that the moves array count the single move loc1
 * @param moves array of moves
 * @param count number of moves in the array
 * @param loc GridLocation that should be in the array
 * @return true of correct size and contents, otherwise false
 */
bool CheckMoves(const GridLocation moves[], size_t count, const GridLocation& loc) {
    return count == 1 && moves[0] == loc;
}

/**
 * Check that the moves array has the two specified entries
 * @param moves array of moves
 * @param count number of moves in the array
 * @param loc1 GridLocation that should be in the array
 * @param loc2 GridLocation that should be in the array
 * @return true of correct size and contents, otherwise false
 */
bool CheckMoves(const GridLocation moves[], size_t count, const GridLocation& loc1, const GridLocation& loc2) {
    if (count == 2) {
        return FindMove(moves, count, loc1) && FindMove(moves, count, loc2);
    }
    return false;
}

/**
 * Check that the moves array has the three specified entries
 * @param moves array of moves
 * @param count number of moves in the array
 * @param loc1 GridLocation that should be in the array
 * @param loc2 GridLocation that should be in the array
 * @param loc3 GridLocation that should be in the array
 * @return true of correct size and contents, otherwise false
 */
bool CheckMoves(const GridLocation moves[], size_t count, const GridLocation& loc1, const GridLocation& loc2, const GridLocation& loc3) {
    if (count == 3) {
        return FindMove(moves, count, loc1) && FindMove(moves, count, loc2) && FindMove(moves, count, loc3);
    }
    return false;
}

/**
 * Check that the moves array has the four specified entries
 * @param moves array of moves
 * @param count number of moves in the array
 * @param loc1 GridLocation that should be in the array
 * @param loc2 GridLocation that should be in the array
 * @param loc3 GridLocation that should be in the array
 * @param loc4 GridLocation that should be in the array
 * @return true of correct size and contents, otherwise false
 */
bool CheckMoves(const GridLocation moves[], size_t count, const GridLocation& loc1, const GridLocation& loc2, const GridLocation& loc3, const GridLocation& loc4) {
    if (count == 4) {
        return FindMove(moves, count, loc1) && FindMove(moves, count, loc2) && FindMove(moves, count, loc3)  && FindMove(moves, count, loc4);
    }
    return false;
}

/**
 * Test the implementation of the function GenerateValidMoves.
 * @param testsPassed running total of number of tests passed, updated upon return
 * @param testsFailed running total of number of tests failed, updated upon return
 */
void TestGenerateValidMoves(unsigned& testsPassed, unsigned& testsFailed) {
    Grid grid;
    GridLocation moves[4];
    size_t count;

    grid.Configure(3,4);

    // Closed corners
    count = 4;
    GenerateValidMoves(grid, GridLocation(0,0), moves, count);
    Test(count == 0, "Test closed corner", testsPassed, testsFailed  );
    count = 4;
    GenerateValidMoves(grid, GridLocation(2,0), moves, count);
    Test(count == 0, "Test closed corner", testsPassed, testsFailed  );
    count = 4;
    GenerateValidMoves(grid, GridLocation(0,3), moves, count);
    Test(count == 0, "Test closed corner", testsPassed, testsFailed  );
    count = 4;
    GenerateValidMoves(grid, GridLocation(2,3), moves, count);
    Test(count == 0, "Test closed corner", testsPassed, testsFailed  );

    // Open corners
    grid[GridLocation(1,0)] = true;
    grid[GridLocation(0,1)] = true;
    count = 4;
    GenerateValidMoves(grid, GridLocation(0,0), moves, count);
    Test(CheckMoves(moves, count, GridLocation(1,0), GridLocation(0,1)), "Test open corner", testsPassed, testsFailed  );
    grid[GridLocation(2,1)] = true;
    grid[GridLocation(1,0)] = true;
    count = 4;
    GenerateValidMoves(grid, GridLocation(2,0), moves, count);
    Test(CheckMoves(moves, count, GridLocation(1,0), GridLocation(2,1)), "Test open corner", testsPassed, testsFailed  );
    grid[GridLocation(0,2)] = true;
    grid[GridLocation(1,3)] = true;
    count = 4;
    GenerateValidMoves(grid, GridLocation(0,3), moves, count);
    Test(CheckMoves(moves, count, GridLocation(1,3), GridLocation(0,2)), "Test open corner", testsPassed, testsFailed  );
    grid[GridLocation(2,2)] = true;
    grid[GridLocation(1,3)] = true;
    count = 4;
    GenerateValidMoves(grid, GridLocation(2,3), moves, count);
    Test(CheckMoves(moves, count, GridLocation(1,3), GridLocation(2,2)), "Test open corner", testsPassed, testsFailed  );

    // Partially open corners
    grid[GridLocation(1,0)] = false;
    grid[GridLocation(0,1)] = true;
    count = 4;
    GenerateValidMoves(grid, GridLocation(0,0), moves, count);
    Test(CheckMoves(moves, count,GridLocation(0,1)), "Test partially open corner", testsPassed, testsFailed  );
    grid[GridLocation(1,0)] = true;
    grid[GridLocation(0,1)] = false;
    count = 4;
    GenerateValidMoves(grid, GridLocation(0,0), moves, count);
    Test(CheckMoves(moves, count, GridLocation(1,0)), "Test partially open corner", testsPassed, testsFailed  );

    grid[GridLocation(2,1)] = false;
    grid[GridLocation(1,0)] = true;
    count = 4;
    GenerateValidMoves(grid, GridLocation(2,0), moves, count);
    Test(CheckMoves(moves, count, GridLocation(1,0)), "Test partially open corner", testsPassed, testsFailed  );
    grid[GridLocation(2,1)] = false;
    grid[GridLocation(1,0)] = true;
    count = 4;
    GenerateValidMoves(grid, GridLocation(2,0), moves, count);
    Test(CheckMoves(moves, count, GridLocation(1,0)), "Test partially open corner", testsPassed, testsFailed  );

    grid[GridLocation(0,2)] = true;
    grid[GridLocation(1,3)] = false;
    count = 4;
    GenerateValidMoves(grid, GridLocation(0,3), moves, count);
    Test(CheckMoves(moves, count, GridLocation(0,2)), "Test partially open corner", testsPassed, testsFailed  );
    grid[GridLocation(0,2)] = false;
    grid[GridLocation(1,3)] = true;
    count = 4;
    GenerateValidMoves(grid, GridLocation(0,3), moves, count);
    Test(CheckMoves(moves, count, GridLocation(1,3)), "Test partially open corner", testsPassed, testsFailed  );

    grid[GridLocation(2,2)] = true;
    grid[GridLocation(1,3)] = false;
    count = 4;
    GenerateValidMoves(grid, GridLocation(2,3), moves, count);
    Test(CheckMoves(moves, count, GridLocation(2,2)), "Test partially open corner", testsPassed, testsFailed  );
    grid[GridLocation(2,2)] = false;
    grid[GridLocation(1,3)] = true;
    count = 4;
    GenerateValidMoves(grid, GridLocation(2,3), moves, count);
    Test(CheckMoves(moves, count, GridLocation(1,3)), "Test partially open corner", testsPassed, testsFailed  );

    // Closed side
    grid[GridLocation(0,0)] = false;
    grid[GridLocation(1,1)] = false;
    grid[GridLocation(0,2)] = false;
    count = 4;
    GenerateValidMoves(grid, GridLocation(0,1), moves, count);
    Test(count == 0, "Test closed side", testsPassed, testsFailed  );
    count = 4;
    grid[GridLocation(2,0)] = false;
    grid[GridLocation(1,1)] = false;
    grid[GridLocation(2,2)] = false;
    GenerateValidMoves(grid, GridLocation(2,1), moves, count);
    Test(count == 0, "Test closed side", testsPassed, testsFailed  );
    count = 4;
    grid[GridLocation(0,0)] = false;
    grid[GridLocation(1,1)] = false;
    grid[GridLocation(2,0)] = false;
    GenerateValidMoves(grid, GridLocation(1,0), moves, count);
    Test(count == 0, "Test closed side", testsPassed, testsFailed  );
    count = 4;
    grid[GridLocation(0,3)] = false;
    grid[GridLocation(1,2)] = false;
    grid[GridLocation(2,3)] = false;
    GenerateValidMoves(grid, GridLocation(1,3), moves, count);
    Test(count == 0, "Test closed side", testsPassed, testsFailed  );

    // Open side
    grid[GridLocation(0,0)] = true;
    grid[GridLocation(1,1)] = true;
    grid[GridLocation(0,2)] = true;
    count = 4;
    GenerateValidMoves(grid, GridLocation(0,1), moves, count);
    Test(CheckMoves(moves, count, GridLocation(0,0), GridLocation(1,1), GridLocation(0,2)), "Test open side", testsPassed, testsFailed  );
    count = 4;
    grid[GridLocation(2,0)] = true;
    grid[GridLocation(1,1)] = true;
    grid[GridLocation(2,2)] = true;
    GenerateValidMoves(grid, GridLocation(2,1), moves, count);
    Test(CheckMoves(moves, count, GridLocation(2,0), GridLocation(1,1), GridLocation(2,2)), "Test open side", testsPassed, testsFailed  );
    count = 4;
    grid[GridLocation(0,0)] = true;
    grid[GridLocation(1,1)] = true;
    grid[GridLocation(2,0)] = true;
    GenerateValidMoves(grid, GridLocation(1,0), moves, count);
    Test(CheckMoves(moves, count, GridLocation(0,0), GridLocation(1,1), GridLocation(2,0)), "Test open side", testsPassed, testsFailed  );
    count = 4;
    grid[GridLocation(0,3)] = true;
    grid[GridLocation(1,2)] = true;
    grid[GridLocation(2,3)] = true;
    GenerateValidMoves(grid, GridLocation(1,3), moves, count);
    Test(CheckMoves(moves, count, GridLocation(0,3), GridLocation(1,2), GridLocation(2,3)), "Test open side", testsPassed, testsFailed  );

    // Partially open side
    grid[GridLocation(0,0)] = false;
    grid[GridLocation(1,1)] = true;
    grid[GridLocation(0,2)] = true;
    count = 4;
    GenerateValidMoves(grid, GridLocation(0,1), moves, count);
    Test(CheckMoves(moves, count, GridLocation(1,1), GridLocation(0,2)), "Test partially open side", testsPassed, testsFailed  );

    grid[GridLocation(0,0)] = true;
    grid[GridLocation(1,1)] = false;
    grid[GridLocation(0,2)] = false;
    count = 4;
    GenerateValidMoves(grid, GridLocation(0,1), moves, count);
    Test(CheckMoves(moves, count, GridLocation(0,0)), "Test partially open side", testsPassed, testsFailed  );

    count = 4;
    grid[GridLocation(2,0)] = true;
    grid[GridLocation(1,1)] = false;
    grid[GridLocation(2,2)] = true;
    GenerateValidMoves(grid, GridLocation(2,1), moves, count);
    Test(CheckMoves(moves, count, GridLocation(2,0), GridLocation(2,2)), "Test partially open side", testsPassed, testsFailed  );

    count = 4;
    grid[GridLocation(2,0)] = false;
    grid[GridLocation(1,1)] = false;
    grid[GridLocation(2,2)] = true;
    GenerateValidMoves(grid, GridLocation(2,1), moves, count);
    Test(CheckMoves(moves, count, GridLocation(2,2)), "Test partially open side", testsPassed, testsFailed  );

    count = 4;
    grid[GridLocation(0,0)] = true;
    grid[GridLocation(1,1)] = true;
    grid[GridLocation(2,0)] = false;
    GenerateValidMoves(grid, GridLocation(1,0), moves, count);
    Test(CheckMoves(moves, count, GridLocation(0,0), GridLocation(1,1)), "Test partially open side", testsPassed, testsFailed  );

    count = 4;
    grid[GridLocation(0,0)] = false;
    grid[GridLocation(1,1)] = true;
    grid[GridLocation(2,0)] = false;
    GenerateValidMoves(grid, GridLocation(1,0), moves, count);
    Test(CheckMoves(moves, count, GridLocation(1,1)), "Test partially open side", testsPassed, testsFailed  );

    count = 4;
    grid[GridLocation(0,3)] = true;
    grid[GridLocation(1,2)] = false;
    grid[GridLocation(2,3)] = false;
    GenerateValidMoves(grid, GridLocation(1,3), moves, count);
    Test(CheckMoves(moves, count, GridLocation(0,3)), "Test partially open side", testsPassed, testsFailed  );

    count = 4;
    grid[GridLocation(0,3)] = false;
    grid[GridLocation(1,2)] = true;
    grid[GridLocation(2,3)] = true;
    GenerateValidMoves(grid, GridLocation(1,3), moves, count);
    Test(CheckMoves(moves, count, GridLocation(1,2), GridLocation(2,3)), "Test partially open side", testsPassed, testsFailed  );

    // Closed middle
    grid[GridLocation(0,1)] = false;
    grid[GridLocation(1,0)] = false;
    grid[GridLocation(2,1)] = false;
    grid[GridLocation(1,2)] = false;
    count = 4;
    GenerateValidMoves(grid, GridLocation(1,1), moves, count);
    Test(count == 0, "Test closed middle", testsPassed, testsFailed  );

    // Open middle
    grid[GridLocation(0,1)] = true;
    grid[GridLocation(1,0)] = true;
    grid[GridLocation(2,1)] = true;
    grid[GridLocation(1,2)] = true;
    count = 4;
    GenerateValidMoves(grid, GridLocation(1,1), moves, count);
    Test(CheckMoves(moves, count, GridLocation(0,1), GridLocation(1,0), GridLocation(2,1), GridLocation(1, 2)), "Test open middle", testsPassed, testsFailed  );

    // Partially open middle
    grid[GridLocation(0,1)] = false;
    grid[GridLocation(1,0)] = true;
    grid[GridLocation(2,1)] = true;
    grid[GridLocation(1,2)] = true;
    count = 4;
    GenerateValidMoves(grid, GridLocation(1,1), moves, count);
    Test(CheckMoves(moves, count, GridLocation(1,0), GridLocation(2,1), GridLocation(1, 2)), "Test open middle", testsPassed, testsFailed  );

    grid[GridLocation(0,1)] = true;
    grid[GridLocation(1,0)] = false;
    grid[GridLocation(2,1)] = false;
    grid[GridLocation(1,2)] = true;
    count = 4;
    GenerateValidMoves(grid, GridLocation(1,1), moves, count);
    Test(CheckMoves(moves, count, GridLocation(0,1), GridLocation(1, 2)), "Test open middle", testsPassed, testsFailed  );

    grid[GridLocation(0,1)] = false;
    grid[GridLocation(1,0)] = true;
    grid[GridLocation(2,1)] = false;
    grid[GridLocation(1,2)] = false;
    count = 4;
    GenerateValidMoves(grid, GridLocation(1,1), moves, count);
    Test(CheckMoves(moves, count, GridLocation(1, 0)), "Test open middle", testsPassed, testsFailed  );
}

/**
 * Test the implementation of the GridLocationList class.  This code doesn't catch memory problems
 * make sure you run under valgrind to find and correct them
 * @param testsPassed running total of number of tests passed, updated upon return
 * @param testsFailed running total of number of tests failed, updated upon return
 */
void TestGridClass(unsigned& testsPassed, unsigned& testsFailed) {
    ifstream ifs;
    stringstream ss;
    Grid grid1;

    Test(grid1.NumberRows() == 0 && grid1.NumberCols() == 0, "Test default constructor", testsPassed, testsFailed);
    Test(!grid1.IsWithinGrid(GridLocation(0,0)) && !grid1.IsWithinGrid(GridLocation(1,1)), "Test IsWithinGrid zero grid", testsPassed, testsFailed);
    grid1.Configure(2, 3);
    Test(grid1.NumberRows() == 2 && grid1.NumberCols() == 3, "Test Configure", testsPassed, testsFailed);
    Test(grid1.IsWithinGrid(GridLocation(0,0)) && grid1.IsWithinGrid(GridLocation(1,2)), "Test IsWithinGrid positive", testsPassed, testsFailed);
    Test(!grid1.IsWithinGrid(GridLocation(2,1)) && !grid1.IsWithinGrid(GridLocation(1,3)), "Test IsWithinGrid negative", testsPassed, testsFailed);
    Test( !grid1[GridLocation(0,0)] && !grid1[GridLocation(0,1)] && !grid1[GridLocation(0,2)] && !grid1[GridLocation(1,0)]
        && !grid1[GridLocation(1,1)] && !grid1[GridLocation(1,0)], "Test [] (get) for empty grid", testsPassed, testsFailed);
    grid1[GridLocation(1,1)] = true;
    grid1[GridLocation(0,2)] = true;
    Test( !grid1[GridLocation(0,0)] && !grid1[GridLocation(0,1)] && grid1[GridLocation(0,2)] && !grid1[GridLocation(1,0)]
          && grid1[GridLocation(1,1)] && !grid1[GridLocation(1,0)], "Test [] (get) for non-empty grid", testsPassed, testsFailed);
    grid1.Configure(1, 4);
    Test(grid1.NumberRows() == 1 && grid1.NumberCols() == 4, "Test (re-)Configure", testsPassed, testsFailed);
    Test(grid1.IsWithinGrid(GridLocation(0,0)) && grid1.IsWithinGrid(GridLocation(0,3)), "Test IsWithinGrid positive", testsPassed, testsFailed);
    Test(!grid1.IsWithinGrid(GridLocation(1,0)) && !grid1.IsWithinGrid(GridLocation(0,4)), "Test IsWithinGrid negative", testsPassed, testsFailed);
    Test( !grid1[GridLocation(0,0)] && !grid1[GridLocation(0,1)]
          && !grid1[GridLocation(0,2)] && !grid1[GridLocation(0,3)], "Test [] (get) for empty grid", testsPassed, testsFailed);
    grid1[GridLocation(0,1)] = true;
    grid1[GridLocation(0,2)] = true;
    Test( !grid1[GridLocation(0,0)] && grid1[GridLocation(0,1)]
          && grid1[GridLocation(0,2)] && !grid1[GridLocation(0,3)], "Test [] (get) for non-empty grid", testsPassed, testsFailed);

    ifs.open("../solvable/5x7.maze", ifstream::in);
    if (!ifs.good()) {
        Test( false, "Test loading ../solvable/5x7.maze - file open failed", testsPassed, testsFailed);
        exit(2);
    }
    if (!grid1.LoadFromFile(ifs)) {
        Test( false, "Test loading ../solvable/5x7.maze - call on LoadFromFile method failed", testsPassed, testsFailed);
        exit(3);
    }
    Test( true, "Test loading ../solvable/5x7.maze", testsPassed, testsFailed);
    ss << grid1;
    Test( ss.str() == "-------\n"
                      "-@@@@@-\n"
                      "-----@-\n"
                      "-@@@-@-\n"
                      "-@---@-\n", "Test maze contents after load", testsPassed, testsFailed);
}

/**
 * Test the implementation of the GridLocationList class.
 * @param testsPassed running total of number of tests passed, updated upon return
 * @param testsFailed running total of number of tests failed, updated upon return
 */
void TestGridLocationClass(unsigned& testsPassed, unsigned& testsFailed) {
    GridLocation def;
    GridLocation gl23(2,3);
    GridLocation gl23a(2,3);
    GridLocation gl26(2,6);
    GridLocation gl13(1,3);

    Test(def.Row() == 0 && def.Col() == 0, "Test default constructor", testsPassed, testsFailed);
    Test(gl23.Row() == 2 && gl23.Col() == 3, "Test specific constructor", testsPassed, testsFailed);
    Test( gl23 == gl23a && gl23 == gl23, "Test == positive", testsPassed, testsFailed);
    Test( !(def == gl23) && !(gl13 == gl23) && !(gl13 == gl26), "Test == negative", testsPassed, testsFailed);
    Test( def < gl13 && gl13 < gl23 && gl23 < gl26, "Test < positive", testsPassed, testsFailed);
    Test( !(gl26 < def) && !(gl23 < gl23a) && !(gl23 < gl13) && !(gl26 < gl23), "Test < negative", testsPassed, testsFailed);
}

/**
 * Test the implementation of the DoubleLinkedList class.  This code doesn't catch memory problems
 * make sure you run under valgrind to find and correct them
 * @param testsPassed running total of number of tests passed, updated upon return
 * @param testsFailed running total of number of tests failed, updated upon return
 */
void Test(bool condition, const char* message, unsigned& testsPassed, unsigned& testsFailed){
    if (condition){
        cout << "+\tTEST PASSED (" << left << setw(70) << message << ") Total Passed: " << ++testsPassed << endl;
    }
    else{
        ++testsFailed;
        cout << "-\tTEST FAILED (" << left << setw(70) << message << ") " << string(25, '*') << "   <----- Failed" << endl;
    }
}

