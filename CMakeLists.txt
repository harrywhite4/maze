cmake_minimum_required(VERSION 3.13)

# Set project name and version
project(maze 
        LANGUAGES CXX
        VERSION 0.1.0
        DESCRIPTION "Generate mazes")

configure_file(src/MazeConfig.h.in src/MazeConfig.h)

# Set required c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compile options
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# Dependencies
add_library(cxxopts INTERFACE)
target_include_directories(cxxopts INTERFACE extern/cxxopts/include)

# Source
add_subdirectory(src)

# Testing
enable_testing()
add_subdirectory(extern/googletest EXCLUDE_FROM_ALL)
add_subdirectory(test)
