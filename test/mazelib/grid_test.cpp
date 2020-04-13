#include "gtest/gtest.h"
#include "mazelib/grid.hpp"

class GridGraphTest : public ::testing::Test {
 protected:
     mazelib::GridGraph graph;

    GridGraphTest() :
        graph(5, 5) {
    }
};


TEST_F(GridGraphTest, TestNumColumns) {
    ASSERT_EQ(graph.getNumColumns(), 5);
}

TEST_F(GridGraphTest, TestNumRows) {
    ASSERT_EQ(graph.getNumRows(), 5);
}

TEST_F(GridGraphTest, TestAddUp) {
    graph.addEdge(6, mazelib::Up);
    ASSERT_TRUE(graph.hasEdge(6, mazelib::Up));
    ASSERT_TRUE(graph.hasEdge(1, mazelib::Down));
}

TEST_F(GridGraphTest, TestAddLeft) {
    graph.addEdge(6, mazelib::Left);
    ASSERT_TRUE(graph.hasEdge(6, mazelib::Left));
    ASSERT_TRUE(graph.hasEdge(5, mazelib::Right));
}

TEST_F(GridGraphTest, TestAddRight) {
    graph.addEdge(6, mazelib::Right);
    ASSERT_TRUE(graph.hasEdge(6, mazelib::Right));
    ASSERT_TRUE(graph.hasEdge(7, mazelib::Left));
}

TEST_F(GridGraphTest, TestAddDown) {
    graph.addEdge(6, mazelib::Down);
    ASSERT_TRUE(graph.hasEdge(6, mazelib::Down));
    ASSERT_TRUE(graph.hasEdge(11, mazelib::Up));
}

TEST_F(GridGraphTest, TestValidEdgeExists) {
    ASSERT_TRUE(graph.edgeExists(0, mazelib::Right));
}

TEST_F(GridGraphTest, TestInvalidEdgeExists) {
    ASSERT_FALSE(graph.edgeExists(0, mazelib::Left));
}

TEST_F(GridGraphTest, TestDirectionUp) {
    ASSERT_EQ(graph.nodeInDirection(6, mazelib::Up), 1);
}

TEST_F(GridGraphTest, TestDirectionDown) {
    ASSERT_EQ(graph.nodeInDirection(6, mazelib::Down), 11);
}

TEST_F(GridGraphTest, TestDirectionLeft) {
    ASSERT_EQ(graph.nodeInDirection(6, mazelib::Left), 5);
}

TEST_F(GridGraphTest, TestDirectionRight) {
    ASSERT_EQ(graph.nodeInDirection(6, mazelib::Right), 7);
}

TEST_F(GridGraphTest, TestCenterPossibleDirs) {
    std::vector<mazelib::Direction> dirs;
    graph.getPossibleDirs(dirs, 6);

    std::vector<mazelib::Direction> expected = {
        mazelib::Left,
        mazelib::Right,
        mazelib::Up,
        mazelib::Down,
    };
    ASSERT_EQ(dirs, expected);
}

TEST_F(GridGraphTest, TestCornerPossibleDirs) {
    std::vector<mazelib::Direction> dirs;
    graph.getPossibleDirs(dirs, 4);

    std::vector<mazelib::Direction> expected = {
        mazelib::Left,
        mazelib::Down,
    };
    ASSERT_EQ(dirs, expected);
}
