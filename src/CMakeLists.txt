# Bitmap library
set(BITMAP_SOURCES
    bitmap/bitmap.cpp 
    bitmap/bitmap.hpp 
    bitmap/image.hpp)

add_library(bitmap ${BITMAP_SOURCES})
target_include_directories(bitmap PUBLIC bitmap)

# Maze executable
set(MAZE_SOURCES
    mazelib/argparse.hpp
    mazelib/argparse.cpp
    mazelib/grid.hpp
    mazelib/grid.cpp
    mazelib/maze.hpp
    mazelib/maze.cpp)

add_library(mazelib ${MAZE_SOURCES})
target_link_libraries(mazelib PRIVATE bitmap)
target_include_directories(mazelib PUBLIC mazelib)
