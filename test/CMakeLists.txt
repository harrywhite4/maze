set(TEST_SOURCES
    test.cpp
    mazelib/grid_test.cpp
    mazelib/maze_test.cpp
)
# Test executable
add_executable(testmaze ${TEST_SOURCES})
target_link_libraries(testmaze PRIVATE gtest_main bitmap mazelib)
target_include_directories(testmaze PRIVATE "${CMAKE_SOURCE_DIR}/src")
# Discover tests from executable
include(GoogleTest)
gtest_discover_tests(testmaze)
