#ifndef MAZE_GRID_H_
#define MAZE_GRID_H_

#include <vector>

enum Direction {Left, Right, Up, Down};

struct OptionalNode {
    bool valid;
    unsigned int node;
};

class GridGraph {
    
    unsigned int numRows;
    unsigned int numColumns;

    std::vector<bool> rightEdges;
    std::vector<bool> downEdges;

    // Set edge from node in direction to value, return whether successful
    bool setEdge(unsigned int node, Direction dir, bool value);

    public:

    GridGraph(unsigned int numRows, unsigned int rowWidth);

    // Whether node number is valid
    bool validNode(unsigned int node);

    // 0 indexed row number
    unsigned int rowNumber(unsigned int node);

    // 0 indexed column number
    unsigned int columnNumber(unsigned int node);

    // Number of nodes in the graph
    unsigned int getNumNodes();
    
    // Number of rows in the graph
    unsigned int getNumRows();

    // Number of nodes in the graph
    unsigned int getNumColumns();

    // Get the node in direction dir from start if valid
    OptionalNode nodeInDirection(unsigned int start, Direction dir);

    // Whether edge in this direction from node exists
    bool hasEdge(unsigned int node, Direction dir);

    // Add an edge from node in this direction, return whether edge was added
    bool addEdge(unsigned int node, Direction dir);

    // Remove an edge from node in this direction, return whether edge was removed
    bool removeEdge(unsigned int node, Direction dir);

};

#endif
