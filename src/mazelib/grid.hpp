#ifndef MAZELIB_GRID_HPP_
#define MAZELIB_GRID_HPP_

#include <vector>
#include <optional>
#include <random>
#include <unordered_set>

namespace mazelib {

enum Direction: unsigned int {Left = 0, Right, Up, Down};

class GridGraph {
    unsigned int numRows;
    unsigned int numColumns;

    // Used to store edge information
    std::vector<bool> horEdges;
    std::vector<bool> vertEdges;

    // Set edge from node in direction to value, return whether successful
    void setEdge(unsigned int node, Direction dir, bool value);

    // Get index in horEdges or vertEdges for an edge if the edge exists
    std::optional<unsigned int> edgeIndex(unsigned int node, Direction dir) const;

    // Throw exception if node is invalid
    void validateNode(unsigned int node) const;

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
    unsigned int nodeInDirection(unsigned int start, Direction dir) const;

    // Returns whether an edge is set
    // This will return false for edges that dont exist within the graph
    bool hasEdge(unsigned int node, Direction dir) const;

    // Add an edge from node in this direction
    void addEdge(unsigned int node, Direction dir);

    // Remove an edge from node in this direction
    void removeEdge(unsigned int node, Direction dir);

    // Get possible directions to move from a given node
    // Does not allow backtracking over set edges
    void getPossibleDirs(std::vector<Direction>& dirs, unsigned int node);

    // Add a random edge from a list of directions
    Direction addRandomEdge(const std::vector<Direction>& dirs,
                            unsigned int node, std::mt19937& gen);
};

} // namespace mazelib

#endif  // MAZELIB_GRID_HPP_
