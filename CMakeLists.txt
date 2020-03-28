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

# Googletest library
add_subdirectory(extern/googletest EXCLUDE_FROM_ALL)
add_subdirectory(extern/cxxopts EXCLUDE_FROM_ALL)

# Libraries
add_subdirectory(src)
# Executables
add_subdirectory(apps)
# Test exceutable
add_subdirectory(test)