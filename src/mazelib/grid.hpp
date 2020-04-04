#ifndef GRID_HPP_
#define GRID_HPP_

#include <vector>
#include <optional>
#include <random>
#include <unordered_set>

enum Direction: unsigned int {Left = 0, Right, Up, Down};

struct Edge {
    unsigned int node;
    Direction dir;
};

class GridGraph {
    unsigned int numRows;
    unsigned int numColumns;

    // Used to store edge information
    std::vector<bool> horEdges;
    std::vector<bool> vertEdges;

    // Set edge from node in direction to value, return whether successful
    bool setEdge(unsigned int node, Direction dir, bool value);

    // Get index in horEdges or vertEdges for an edge
    std::optional<unsigned int> edgeIndex(unsigned int node, Direction dir) const;

 public:
    GridGraph(unsigned int numRows, unsigned int rowWidth);

    // Whether node number is valid
    bool validNode(unsigned int node) const;

    // 0 indexed row number
    unsigned int rowNumber(unsigned int node) const;

    // 0 indexed column number
    unsigned int columnNumber(unsigned int node) const;

    // Get node number from row and column number
    unsigned int nodeNumber(unsigned int rowNum, unsigned int columnNum) const;

    // Number of nodes in the graph
    unsigned int getNumNodes() const;

    // Number of rows in the graph
    unsigned int getNumRows() const;

    // Number of columns in the graph
    unsigned int getNumColumns() const;

    // Get the node in direction dir from start if valid
    std::optional<unsigned int> nodeInDirection(unsigned int start, Direction dir) const;

    // Whether edge in this direction from node exists
    bool hasEdge(unsigned int node, Direction dir) const;

    // Add an edge from node in this direction, return whether edge was added
    bool addEdge(unsigned int node, Direction dir);

    // Remove an edge from node in this direction, return whether edge was removed
    bool removeEdge(unsigned int node, Direction dir);

    // Get possible dirs
    void getPossibleDirs(std::vector<Direction>& dirs, unsigned int node);

    // Add a random edge in direction
    std::optional<Direction> addRandomEdge(const std::vector<Direction>& dirs, unsigned int node, std::mt19937& gen);
};

#endif  // GRID_HPP_
