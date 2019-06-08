#include <vector>
#include <optional>
#include <random>

#include "grid.hpp"

GridGraph::GridGraph(unsigned int numRows, unsigned int numColumns) :
    numRows(numRows),
    numColumns(numColumns),
    rightEdges(numColumns * numRows, false),
    downEdges(numColumns * numRows, false),
    possibleDirs(4) {
}

bool GridGraph::validNode(unsigned int node) const {
    return (node < getNumNodes());
}

unsigned int GridGraph::getNumNodes() const {
    return numRows * numColumns;
}

unsigned int GridGraph::getNumRows() const {
    return numRows;
}

unsigned int GridGraph::getNumColumns() const {
    return numColumns;
}

unsigned int GridGraph::rowNumber(unsigned int node) const {
    return node / numColumns;
}

unsigned int GridGraph::columnNumber(unsigned int node) const {
    return node % numColumns;
}

std::optional<unsigned int> GridGraph::nodeInDirection(unsigned int start, Direction dir) const {
    if (validNode(start)) {
        unsigned int rowNum = rowNumber(start);
        switch (dir) {
            case Left:
                if (validNode(start-1)) {
                    return start-1;
                }
            case Right:
                if (validNode(start+1)) {
                    return start+1;
                }
            case Up:
                if (rowNum-1 >= 0) {
                    return (rowNum-1) + columnNumber(start);
                }
            case Down:
                if (rowNum+1 < numRows) {
                    return (rowNum+1) + columnNumber(start);
                }
        }
    }
    return {};
}

bool GridGraph::hasEdge(unsigned int node, Direction dir) const {
    if (!validNode(node)) {
        return false;
    }

    switch (dir) {
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

    switch (dir) {
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

std::optional<Direction> GridGraph::addRandomEdge(unsigned int node, std::mt19937& gen) {
    // clear possible dirs
    possibleDirs.clear();
    // Get possible directions
    auto row = rowNumber(node);
    auto column = columnNumber(node);
    Direction dir;
    if (column > 0 && !hasEdge(node, Left)) {
        possibleDirs.push_back(Left);
    }
    if (column < numColumns-1 && !hasEdge(node, Right)) {
        possibleDirs.push_back(Right);
    }
    if (row > 0 && !hasEdge(node, Up)) {
        possibleDirs.push_back(Up);
    }
    if (row < numRows-1 && !hasEdge(node, Down)) {
        possibleDirs.push_back(Down);
    }
    if (!possibleDirs.empty()) {
        auto size = possibleDirs.size();
        dir = possibleDirs[0];
        // Choose random if there are more than 1
        if (size > 1) {
            std::uniform_int_distribution<> dis(0, possibleDirs.size()-1);
            auto index = dis(gen);
            dir = possibleDirs[index];
        }
        bool success = addEdge(node, dir);
        if (success) {
            return dir;
        }
    }
    return {};
}
