#include <sstream>

#include "gtest/gtest.h"
#include "mazelib/maze.hpp"

// Validate that a given graph is a spanning tree and therefore a valid maze
void validateSpanning(const mazelib::GridGraph& graph) {
    int numEdges = 0;
    std::unordered_set<unsigned int> nodes;
    mazelib::Direction checkDirections[] = {mazelib::Right, mazelib::Down};
    unsigned int numNodes = graph.getNumNodes();

    for (unsigned int node = 0; node < numNodes; ++node) {
        for (mazelib::Direction dir : checkDirections) {
            if (graph.edgeExists(node, dir) && graph.hasEdge(node, dir)) {
                numEdges += 1;
                nodes.insert(node);
                nodes.insert(graph.nodeInDirection(node, dir));
            }
        }
    }

    // Make sure the graph has the correct number of edges
    ASSERT_EQ(numEdges, (numNodes - 1));

    // Make sure all nodes are part of graph
    ASSERT_EQ(nodes.size(), numNodes);
}

TEST(MazeTest, GenerateSmallLerwMaze) {
    mazelib::GridGraph graph(10, 10);
    mazelib::lerwGraph(graph);
    validateSpanning(graph);
}

TEST(MazeTest, GenerateRectangularLerwMaze) {
    mazelib::GridGraph graph(10, 30);
    mazelib::lerwGraph(graph);
    validateSpanning(graph);
}

TEST(MazeTest, GenerateSmallDfsMaze) {
    mazelib::GridGraph graph(10, 10);
    mazelib::dfsGraph(graph);
    validateSpanning(graph);
}

TEST(MazeTest, GenerateRectagularDfsMaze) {
    mazelib::GridGraph graph(10, 30);
    mazelib::dfsGraph(graph);
    validateSpanning(graph);
}

TEST(MazeTest, StreamInputMazeType) {
    std::stringstream ss;
    ss << "wilsons";
    mazelib::MazeType type;
    ss >> type;
    ASSERT_EQ(type, mazelib::Wilsons);
}
