#ifndef GRID_HPP_
#define GRID_HPP_

#include <vector>
#include <optional>
#include <random>

enum Direction: unsigned int {Left = 0, Right, Up, Down};

class GridGraph {
    unsigned int numRows;
    unsigned int numColumns;

    // Used to store edge information
    std::vector<bool> rightEdges;
    std::vector<bool> downEdges;

    // Used to store possible dirs when adding random edge
    std::vector<Direction> possibleDirs;

    // Set edge from node in direction to value, return whether successful
    bool setEdge(unsigned int node, Direction dir, bool value);

    public:
    GridGraph(unsigned int numRows, unsigned int rowWidth);

    // Whether node number is valid
    bool validNode(unsigned int node) const;

    // 0 indexed row number
    unsigned int rowNumber(unsigned int node) const;

    // 0 indexed column number
    unsigned int columnNumber(unsigned int node) const;

    // Number of nodes in the graph
    unsigned int getNumNodes() const;

    // Number of rows in the graph
    unsigned int getNumRows() const;

    // Number of nodes in the graph
    unsigned int getNumColumns() const;

    // Get the node in direction dir from start if valid
    std::optional<unsigned int> nodeInDirection(unsigned int start, Direction dir) const;

    // Whether edge in this direction from node exists
    bool hasEdge(unsigned int node, Direction dir) const;

    // Add an edge from node in this direction, return whether edge was added
    bool addEdge(unsigned int node, Direction dir);

    // Add a random edge
    std::optional<Direction> addRandomEdge(unsigned int node, std::mt19937& gen);

    // Remove an edge from node in this direction, return whether edge was removed
    bool removeEdge(unsigned int node, Direction dir);
};

#endif  // GRID_HPP_
