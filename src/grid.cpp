#include <vector>
#include <optional>
#include <random>
#include <iostream>

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
        unsigned int colNum = columnNumber(start);
        switch (dir) {
            case Left:
                if (colNum > 0) {
                    return start-1;
                }
                break;
            case Right:
                if ((colNum+1) < numColumns) {
                    return start+1;
                }
                break;
            case Up:
                if (rowNum > 0) {
                    return ((rowNum-1)*numColumns) + colNum;
                }
                break;
            case Down:
                if ((rowNum+1) < numRows) {
                    return ((rowNum+1)*numColumns) + colNum;
                }
                break;
        }
    }
    return {};
}

bool GridGraph::hasEdge(unsigned int node, Direction dir) const {
    if (!validNode(node)) {
        return false;
    }

    int columnNum = columnNumber(node);
    switch (dir) {
    case Right:
        return rightEdges[node];
        break;
    case Left:
        if (columnNum > 0) {
            return rightEdges[node-1];
        }
        break;
    case Down:
        return downEdges[node];
        break;
    case Up:
        int rowAbove = rowNumber(node) - 1;
        if (rowAbove >= 0) {
            return downEdges[rowAbove + columnNum];
        }
        break;
    }
    return false;
}

bool GridGraph::setEdge(unsigned int node, Direction dir, bool value) {
    if (!validNode(node)) {
        return false;
    }

    unsigned int columnNum = columnNumber(node);
    switch (dir) {
    case Right:
        rightEdges[node] = value;
        return true;
        break;
    case Left:
        if (columnNum > 0) {
            rightEdges[node-1] = value;
            return true;
        }
        break;
    case Down:
        downEdges[node] = value;
        return true;
        break;
    case Up:
        int rowAbove = rowNumber(node) - 1;
        if (rowAbove >= 0) {
            downEdges[rowAbove + columnNum] = value;
            return true;
        }
        break;
    }
    return false;
}

bool GridGraph::addEdge(unsigned int node, Direction dir) {
    return setEdge(node, dir, true);
}

bool GridGraph::removeEdge(unsigned int node, Direction dir) {
    return setEdge(node, dir, false);
}

void GridGraph::setPossibleDirs(unsigned int node) {
    possibleDirs.clear();
    auto row = rowNumber(node);
    auto column = columnNumber(node);
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
}

bool GridGraph::directionIsExcluded(unsigned int node, Direction dir,
                         const std::unordered_set<unsigned int>& exclusions) {
        auto onode = nodeInDirection(node, dir);
        // If not valid direction
        if (!onode.has_value()) {
            return true;
        }
        // std::cout << dir << " is valid for " << node << "\n";
        // If there is a node in that direction that is excluded
        if (exclusions.count(onode.value()) > 0) {
            return true;
        }
        return false;
}

void GridGraph::setPossibleDirsExclusions(unsigned int node,
                         const std::unordered_set<unsigned int>& exclusions) {
    possibleDirs.clear();
    if (!directionIsExcluded(node, Left, exclusions) && !hasEdge(node, Left)) {
        possibleDirs.push_back(Left);
    }
    if (!directionIsExcluded(node, Right, exclusions) && !hasEdge(node, Right)) {
        possibleDirs.push_back(Right);
    }
    if (!directionIsExcluded(node, Up, exclusions) && !hasEdge(node, Up)) {
        possibleDirs.push_back(Up);
    }
    if (!directionIsExcluded(node, Down, exclusions) && !hasEdge(node, Down)) {
        possibleDirs.push_back(Down);
    }
}

std::optional<Direction> GridGraph::addRandomDirection(unsigned int node, std::mt19937& gen) {
    if (!possibleDirs.empty()) {
        auto size = possibleDirs.size();
        auto dir = possibleDirs[0];
        // Choose random if there are more than 1
        if (size > 1) {
            std::uniform_int_distribution<> dis(0, possibleDirs.size()-1);
            dir = possibleDirs[dis(gen)];
        }
        auto success = addEdge(node, dir);
        if (success) {
            return dir;
        } else {
            std::cout << "Could not add edge " << dir << " from " << node << "\n";
        }
    }
    return {};
}

std::optional<Direction> GridGraph::addRandomEdge(unsigned int node, std::mt19937& gen) {
    setPossibleDirs(node);
    return addRandomDirection(node, gen);
}

std::optional<Direction> GridGraph::addRandomEdgeWithExclusions(unsigned int node,
                     std::mt19937& gen, const std::unordered_set<unsigned int>& exclusions) {
    setPossibleDirsExclusions(node, exclusions);
    auto dir = addRandomDirection(node, gen);
    if (dir.has_value()) {
        std::cout << "added edge in dir " << dir.value() << " from " << node << "\n";
    }
    return dir;
}
