add_executable(maze main.cpp)
target_link_libraries(maze cxxopts bitmap mazelib)

# Include config header
target_include_directories(maze PUBLIC "${PROJECT_BINARY_DIR}/apps")
