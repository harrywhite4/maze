#include "grid.hpp"
#include "gtest/gtest.h"

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

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
