#include <vector>

#include "grid.hpp"

GridGraph::GridGraph(unsigned int numRows, unsigned int numColumns) : 
    numRows(numRows), 
    numColumns(numColumns),
    rightEdges(numColumns * numRows, false),
    downEdges(numColumns * numRows, false) {
}

bool GridGraph::validNode(unsigned int node) {
    return (node < getNumNodes());
}

unsigned int GridGraph::getNumNodes() {
    return numRows * numColumns;
}

unsigned int GridGraph::getNumRows() {
    return numRows;
}

unsigned int GridGraph::getNumColumns() {
    return numColumns;
}

unsigned int GridGraph::rowNumber(unsigned int node) {
    return node / numColumns;
}

unsigned int GridGraph::columnNumber(unsigned int node) {
    return node % numColumns;
}

OptionalNode GridGraph::nodeInDirection(unsigned int start, Direction dir) {
    if (validNode(start)) {
        unsigned int rowNum = rowNumber(start);
        switch(dir) {
            case Left:
                if (validNode(start-1)) {
                    return {true, start-1};
                }
            case Right:
                if (validNode(start+1)) {
                    return {true, start+1};
                }
            case Up:
                if (rowNum-1 >= 0) {
                    return {true, (rowNum-1) + columnNumber(start)};
                }
            case Down:
                if (rowNum+1 < numRows) {
                    return {true, (rowNum+1) + columnNumber(start)};
                }
        }
    }
    return {false, 0};
}

bool GridGraph::hasEdge(unsigned int node, Direction dir) {
    if (!validNode(node)) {
        return false;
    }

    switch(dir) {
    case Right:
        return rightEdges[node];
    case Left:
        if (node-1 >= 0) {
            return rightEdges[node-1];
        }
    case Down:
        return downEdges[node];
    case Up:
        unsigned int rowAbove = rowNumber(node) - 1;
        if (rowAbove >= 0) {
            return downEdges[rowAbove + columnNumber(node)];
        }
    }
    return false;
}

bool GridGraph::setEdge(unsigned int node, Direction dir, bool value) {
    if (!validNode(node)) {
        return false;
    }

    switch(dir) {
    case Right:
        rightEdges[node] = value;
        return true;
    case Left:
        if (node-1 >= 0) {
            rightEdges[node-1] = value;
            return true;
        }
    case Down:
        downEdges[node] = value;
        return true;
    case Up:
        unsigned int rowAbove = rowNumber(node) - 1;
        if (rowAbove >= 0) {
            downEdges[rowAbove + columnNumber(node)] = value;
            return true;
        }
    }
    return false;
}

bool GridGraph::addEdge(unsigned int node, Direction dir) {
    return setEdge(node, dir, true);
}

bool GridGraph::removeEdge(unsigned int node, Direction dir) {
    return setEdge(node, dir, false);
}
