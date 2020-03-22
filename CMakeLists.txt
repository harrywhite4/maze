cmake_minimum_required(VERSION 3.13)

# Set project name and version
project(maze VERSION 0.1 LANGUAGES CXX)

# Set required c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Conpile options
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    # Look at adding -Wextra
    add_compile_options(-Wall -pedantic -Werror)
endif()

# Libraries
add_library(argparse src/argparse.cpp src/argparse.hpp)
add_library(bitmap src/bitmap.cpp src/bitmap.hpp)
add_library(grid src/grid.cpp src/grid.hpp)

# Maze executable
add_executable(maze src/main.cpp src/maze.cpp src/maze.hpp)
target_include_directories(maze PUBLIC src)
target_link_libraries(maze PUBLIC argparse bitmap grid)

# Googletest
add_subdirectory(extern/googletest EXCLUDE_FROM_ALL)
add_executable(test test/test.cpp test/grid_test.cpp)
target_include_directories(test PUBLIC src)
target_link_libraries(test PUBLIC gtest_main argparse bitmap grid)
