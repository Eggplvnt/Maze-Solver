cmake_minimum_required(VERSION 3.10)
project(MazeSolver)

set(CMAKE_CXX_STANDARD 14)

add_executable(MazeSolver main.cpp Maze.cpp GridLocation.cpp Grid.cpp CursesWindow.cpp)
target_link_libraries(MazeSolver ncurses)