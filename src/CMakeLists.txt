# Bitmap library
set(BITMAP_SOURCES
    bitmap/bitmap.cpp 
    bitmap/bitmap.hpp 
    bitmap/image.hpp
)

add_library(bitmap ${BITMAP_SOURCES})
target_include_directories(bitmap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Maze library
set(MAZE_SOURCES
    mazelib/grid.hpp
    mazelib/grid.cpp
    mazelib/maze.hpp
    mazelib/maze.cpp
    mazelib/error.hpp
)

add_library(mazelib ${MAZE_SOURCES})
target_link_libraries(mazelib PRIVATE bitmap)
target_include_directories(mazelib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Maze executable
add_executable(maze main.cpp)
target_link_libraries(maze cxxopts bitmap mazelib)
# Include config header
target_include_directories(maze PUBLIC "${PROJECT_BINARY_DIR}/src")
# Ouput to bin directory
set_target_properties(maze PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
# Install to bin
install(TARGETS maze DESTINATION bin)
