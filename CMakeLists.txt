cmake_minimum_required(VERSION 3.13)

# Set project name and version
project(maze VERSION 0.1)

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

# Add argparse lib
add_library(argparse src/argparse.cpp src/argparse.hpp)

# Add bitmap lib
add_library(bitmap src/bitmap.cpp src/bitmap.hpp)

# Add grid lib
add_library(grid src/grid.cpp src/grid.hpp)

# Add executable
add_executable(maze src/main.cpp src/maze.cpp src/maze.hpp)

# Use src as include dir
target_include_directories(maze PUBLIC src)

# Link libraries
target_link_libraries(maze PUBLIC argparse bitmap grid)
