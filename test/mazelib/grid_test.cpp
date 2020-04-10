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
