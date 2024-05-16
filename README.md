# Maze Solver

Your job is to find a route through a maze, starting at the upper left corner of the maze, and ending at the lower right corner.  When you get your code working, you will be able to visualize the work your code does to  find the route.

You will use the _stack_, _queue_, and _set_ classes from the C++ Standard Template Library (STL) in your implementation.

## Step 0:  Update the file header comments

Add your name and date to the file headr comments for **GridLocation.h**, **Grid.cpp**, **Grid.h**, and **Maze.cpp**

## Step 1:  Understand how our maze will be represented

A maze can be thought of as a two-dimensional array where each element is either a corridor or a wall. In our programming model, a _true_ value will represent a corridor and a _false_ value represents a wall.  

Consider the 5x7 maze pictured below where light gray represents a corridor, and black represents a wall.

![image](https://user-images.githubusercontent.com/58283777/138574967-13fbe9b7-ab03-4868-acaf-22323cae27de.png)

Its two dimensional array representation would be:

![image](https://user-images.githubusercontent.com/58283777/138965299-97859ca5-2337-4ec2-a810-07aceccd0c71.png)

You will be implementing two classes to model the maze:

- _GridLocation_ this class is used to specify a particular location on the grid.  The class stores the specific row number and column number of the location.

- _Grid_  this class is used to represent the grid itself.  Internally it is implemented using a two-dimensional array of bool values, but a programmer determines whether a particular grid location is a corrior or a wall, but indexing with a _GridLocation_ object representing the location you are interested in. 

For instance, if you wanted to find whether the location on the 2nd row and the 3rd column was corridor, you could find that out using the following code:

```
GridLocation loc(2,3);
if ( maze[loc] ) {
	cout << "Corridor" << endl;
}
else {
	cout << "Wall" << endl;
}
```

The _Grid_ class has a useful method called  _IsWithinGrid_ that you will use to determine whether a particular _GridLocation_ is actually within the grid boundaries.  

When you specify a maze file for the program to solve, the main program calls _grid.LoadFromFile_ method to read the maze information into the _grid_.

## Step 2:  Implement the _GridLocation_ class

The declaration of this class is in **GridLocation.h**:

```
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
```

The `ToString` method has already been implemented for you.  Implement all the rest of the methods.  They don't take much code and are straightforward.  

Notes:
1)  The default constructor should set `_row` and `_col` equal to zero.
2)  The `<` operator should return _true_ for either of the following cases, but _false_ otherwise:
- The `_row` value of `lhs` is less than the `_row` value of `rhs` 
- The `_row` values of `lhs` and `rhs` are equal and the `_col` value of `lhs` is less than the `_col` value of `_rhs`.

The reason that we are implementing a `<` operator is because we are going to store `GridLocation` objects in a STL `set`, and STL requires that we have a `<` operator.

After finishing implementing this class, make sure you pass all the test cases when you run with the **--test:location** command line argument.

## Step 3:  Implement the _Grid_ class

The declaration of this class is in **Grid.h**:

```
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
        for (size_t row = 0; row < grid._nRows; row ++) {
            for (size_t col = 0; col < grid._nCols; col ++) {
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

    // Add your declaration of the data structure you want to use for storing
    // the "grid" of true and false values here
    
    
    //
    
    size_t _nRows;
    size_t _nCols;
};
```

Here's how this class is used by the maze solver:
1) A `Grid` object will be declared.  The default constructor will set it up as an empty grid.
2) The `LoadFromFile` method will be called to populate the grid with _true_ and _false_ values that describe the maze.  As part of the loading process: 
- The dimensions of the grid are read from the file.  
- The `Configure` method is called to allocate storage for the grid and initialize all grid cells to false.  
- The file is read and the grid cell values are set to their correct values.

You get to choose the data structure for storing the _true_/_false_ values.  It could be a dynamic two dimensional array, or you could pack them into a one dimensional array and calculate an index from the row and column stored in a `GridLocation` object.  But your `Configure` methods must be able to dynamically size your data structure to fit any maze.  Add your declaration to **Grid.h** between the comments inside the private section of the class declaration.

`LoadFromFile` has been allocated for you, as has the method to print the grid contents.  Allocate the remaining methods.

Notes:
1) The `Configure` method should be able to be called multiple times without introducing a memory leak. 
2) We are overloading the `[]` operator so that we can "index" into a grid using a `GridLocation` object.  There are two operator overloads: one is for the case where you are getting the value of a grid cell, for instance as part of an condition for an if statement or on the right hand side of an assignment statement.  The other is when you are assigning a value to a grid cell.  The code for both operator overloads should be the same.  Just return the the location where the value is stored.

After finishing implementing this class, make sure you pass all the test cases when you run with the **--test:grid** command line argument.

In addition, run this test case under **valgrind** to make sure you don't have memory leaks or other memory handling problems.

## Step 4:  Write the _GenerateValidMoves_ function

In the file `Maze.cpp`, you should implement the function
  
  `void GenerateValidMoves(const Grid& maze, const GridLocation& loc, GridLocation moves[], size_t& count);`  

Given a grid location, there are always four adjacent grid locations to consider: north, east, south, and west.  But some of these grid locations may be invalid moves because the location is outside the grid or there is a wall at that location.

This function determines the grid locations which are valid moves and copies them into the array parameter `moves`.  It also sets `count` to the number of moves it found.  

After finishing implementing this function, make sure you pass all the test cases when you run with the **--test:moves** command line argument.

## Step 5:  Representing a path through the maze

A path through the maze is a sequence of adjacent grid locations that connect to each other.  We will store the sequence in a _stack<GridLocation>_ using the STL stack template class.  When we want to extend the path to reach the next grid location, we will push the new _GridLocation_ on to the path.

## Step 6:  Write the _CheckSolution_ function

In the file `Maze.cpp`, you should implement the function
  
  `bool CheckSolution(const Grid& maze, Stack<GridLocation> path)`.  
  
The main program uses this function  to determine whether a path is actually a solution to the maze.  
  
Here are the things you need to verify:
- The path must start at the entry (upper left corner) of the maze
- The path must end at the exit (lower right corner) of the maze
- Each location in the path is a valid move from the previous location in the path
- The path must not contain a loop, i.e. the same location cannot appear more than once in the path

As described in the last step, the path is a stack of grid locations.  At the top of the stack is the end of the path.  As you pop grid locations from the stack, you will walk the path back to the starting location.  When you start you can verify that the top of the stack is the lower right corner of the grid and that this is not a wall.  As you pop grid locations you can check that the next grid location is a valid move from the previous one.  When you get to the bottom of stack, you can verify that you have the grid location of the upper right corner.
	
How do you check that the same location appears more than once? One way would be to declare a variable of type `set<GridLocation>`.  Each time you pop a grid location off the stack, you can check if it is already in the set.  If so you have found a loop, otherwise you add this grid location to the set and continue.
	
To use the STL set template, you will probably want to use the `insert` method for adding the grid location to the set, and use the 	`count` method to determine if the grid location is already an element of set.
	
After finishing implementing this function, make sure you pass all the test cases when you run with the **--test:checksolution** command line argument.

## Step 7: Write the _SolveMaze_ function

Now you are ready to implement the function 
  
  `bool SolveMaze(const Grid& maze, stack<GridLocation>& solution, CursesWindow* pwindow)`.  

This function tries to find a solution to the maze.  If it finds one, it will return _true_ and the solution comes back to the caller via the reference parameter _solution_. If no solution is found, it will return _false_.  The parameter _pwindow_ is used for displaying the paths as your algorithm considers them as possibilities.

Your function will initialize a _queue_ of partial paths to drive its work.  Because the queue stores paths, its type will be `queue<stack<GridLocation>>`.  Your function will loop, removing one path from the queue on each iteration.  It will try to extend that path by one additional square.  There may be no way to extend the path further, or multiple ways.  Any path extensions that it does find will be put back on the queue to be considered on another iteration.  In this way, if a path to the ending point exists, it will eventually be found.  In computer science the name _breadth first search_ is given to the type of algorithm that is being used here.

In order to avoid revisiting grid locations that you've considered before you, will also keep track of the _set_ of grid locations  that you have already visited.  When you try to extend a path, if the move is to a  _GridLocation_ that is in this set, then you should not try to extend the path in that direction because you have already found another path that reached that location.  

Here's a sketch of how the algorithm works:
```
1. If the upper left square is a wall, return _false_.
2. Initialize the queue to the path of length 1 that contains the upper left square.
3. Initialize the set of grid locations that you have visited to be the set containing the grid location of the upper left square.
4. Loop until the queue is empty
  a.  Take a path from the queue
  b.  Add this code to display the path momentarily when program is run with -visualize flag:
        if (pwindow) {
            pwindow->ShowPath(path, false);
        }
	c.  Generate set of the valid moves from the _GridLocation_ at the end of the path (i.e. the top item of the stack)
	d.  For each valid move in this set which is not in the set of visited grid locations
		  i.  Generate a new path which consists of the path under consideration plus a move to this grid location
		  ii. If the end of this path is the lower right corner, return true, this path is your solution
		  iii. Otherwise put your new path back on the queue so it can be extended further
	    	  iv. Mark this grid location as visited by adding it the set of visited grid locations
5. If you end up here, you didn't find a solution, so you better return false
```

After finishing implementing this function, make sure you pass all the test cases when you run with the **--test:solve** command line argument.
	
Make sure you run it with `valgrind --leak-check=full` to make sure it does not report any memory problems.

You can also test your code on individual maze files.  Mazes with solutions are located in the `solvable` subdirectory, and mazes with no solution are located in the `unsolvable` directory.  It's also easy to make up your own.

Here is an example of how you can run your program on an individual maze file.  To run your progra on 2x2 solvable maze, run your program like this:

`./MazeSolver ../solvable/2x2.maze`

Or to run your pogram with the 2x2 unsolvable maze, run your program like this:

`./MazeSolver ../unsolvable/2x2.maze`

You can also run your code and see the solution process animated.  You do this by specifying the `-visualize` flag on the command line.  For example,

`./MazeSolver -visualize ../solvable/17x37.maze`
 
Then sit back and enjoy.  When the animation is done, press the space bar to end the program.ls

### Grading

Grading will be based on 
10% --test:location
20% --test:grid
10% --test:moves
30% --test:checksolution
20% --test:solve
 5% testing with mazes that are supplied with the project
 5% - --visualize works

There will be deductions for problems such as:
- run time errors
- errors and memory leaks reported by valgrind
- modifying any repository files other than **GridLocation.cpp**, **Grid.cpp**, **Grid.h** **Maze.cpp**.  The public methods of **Grid.h** may NOT be changed.
- not editing the file header comments for *GridLocation.cpp**, **Grid.cpp**, **Grid.h**, and **Maze.cpp** 
- bad coding practices

