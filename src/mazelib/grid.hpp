#ifndef MAZELIB_GRID_HPP_
#define MAZELIB_GRID_HPP_

#include <vector>
#include <optional>
#include <random>
#include <unordered_set>

namespace mazelib {

enum Direction: int {Left = 0, Right, Up, Down};

class GridGraph {
    int numRows;
    int numColumns;

    // Used to store edge information
    std::vector<bool> horEdges;
    std::vector<bool> vertEdges;

    // Set edge from node in direction to value, return whether successful
    void setEdge(int node, Direction dir, bool value);

    // Get index in horEdges or vertEdges for an edge if the edge exists
    std::optional<int> edgeIndex(int node, Direction dir) const;

    // Throw exception if node is invalid
    void validateNode(int node) const;

    // Whether an edge exists for specified vertex
    bool vertexEdgeExists(int rowNum, int colNum, Direction dir) const;

 public:
    GridGraph(int numRows, int rowWidth);

    // Whether node number is valid
    bool validNode(int node) const;

    // 0 indexed row number
    int rowNumber(int node) const;

    // 0 indexed column number
    int columnNumber(int node) const;

    // Get node number from row and column number
    int nodeNumber(int rowNum, int columnNum) const;

    // Number of nodes in the graph
    int getNumNodes() const;

    // Number of rows in the graph
    int getNumRows() const;

    // Number of columns in the graph
    int getNumColumns() const;

    // Get the node in direction dir from start if valid
    int nodeInDirection(int start, Direction dir) const;

    // Whether an edge exists within the graph
    bool edgeExists(int node, Direction dir) const;

    // Returns whether an edge is set
    // This will return false for edges that dont exist within the graph
    bool hasEdge(int node, Direction dir) const;

    // Add an edge from node in this direction
    void addEdge(int node, Direction dir);

    // Remove an edge from node in this direction
    void removeEdge(int node, Direction dir);

    // Get possible directions to move from a given node
    // Does not allow backtracking over set edges
    void getPossibleDirs(std::vector<Direction>& dirs, int node);

    // Add a random edge from a list of directions
    Direction addRandomEdge(const std::vector<Direction>& dirs, int node, std::mt19937& gen);
};

} // namespace mazelib

#endif  // MAZELIB_GRID_HPP_
