set(
    TEST_SOURCES
    test.cpp
    mazelib/grid_test.cpp
    mazelib/maze_test.cpp
)
# Test executable
add_executable(testmaze ${TEST_SOURCES})
target_link_libraries(testmaze PUBLIC gtest_main bitmap mazelib)
