#include "gtest/gtest.h"
#include "grid.hpp"

class GridGraphTest : public ::testing::Test {
 protected:
    GridGraph graph;

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
    ASSERT_TRUE(graph.addEdge(6, Up));
    ASSERT_TRUE(graph.hasEdge(6, Up));
    ASSERT_TRUE(graph.hasEdge(1, Down));
}

TEST_F(GridGraphTest, TestAddLeft) {
    ASSERT_TRUE(graph.addEdge(6, Left));
    ASSERT_TRUE(graph.hasEdge(6, Left));
    ASSERT_TRUE(graph.hasEdge(5, Right));
}

TEST_F(GridGraphTest, TestAddRight) {
    ASSERT_TRUE(graph.addEdge(6, Right));
    ASSERT_TRUE(graph.hasEdge(6, Right));
    ASSERT_TRUE(graph.hasEdge(7, Left));
}

TEST_F(GridGraphTest, TestAddDown) {
    ASSERT_TRUE(graph.addEdge(6, Down));
    ASSERT_TRUE(graph.hasEdge(6, Down));
    ASSERT_TRUE(graph.hasEdge(11, Up));
}
