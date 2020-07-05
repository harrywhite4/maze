set(TEST_SOURCES
    test.cpp
    mazelib/grid_test.cpp
    mazelib/maze_test.cpp
)
# Test executable
add_executable(testmaze ${TEST_SOURCES})
target_link_libraries(testmaze PUBLIC gtest_main bitmap mazelib)
# Ouput to bin directory
set_target_properties(testmaze PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
