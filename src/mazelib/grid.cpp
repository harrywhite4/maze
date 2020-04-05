#include <vector>
#include <optional>
#include <random>
#include <iostream>

#include "mazelib/grid.hpp"

GridGraph::GridGraph(unsigned int numRows, unsigned int numColumns) :
    numRows(numRows),
    numColumns(numColumns),
    horEdges((numColumns - 1) * numRows, false),
    vertEdges(numColumns * (numRows - 1), false) {
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

unsigned int GridGraph::nodeNumber(unsigned int rowNum, unsigned int columnNum) const {
    return (rowNum * numColumns) + columnNum;
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
                if (colNum < numColumns + 1) {
                    return start+1;
                }
                break;
            case Up:
                if (rowNum > 0) {
                    return nodeNumber(rowNum - 1, colNum);
                }
                break;
            case Down:
                if ((rowNum+1) < numRows) {
                    return nodeNumber(rowNum + 1, colNum);
                }
                break;
        }
    }
    return {};
}

std::optional<unsigned int> GridGraph::edgeIndex(unsigned int node, Direction dir) const {
    if (validNode(node)) {
        unsigned int rowNum = rowNumber(node);
        unsigned int columnNum = columnNumber(node);
        switch (dir) {
            case Right:
                if (columnNum < numColumns - 1) {
                    return node - rowNum;
                }
                break;
            case Left:
                if (columnNum > 0) {
                    return node - rowNum - 1;
                }
                break;
            case Down:
                if (rowNum < numRows - 1) {
                    return node;
                }
                break;
            case Up:
                if (rowNum > 0) {
                    return nodeNumber(rowNum - 1, columnNum);
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

    auto index = edgeIndex(node, dir);
    if (index.has_value()) {
        if (dir == Left || dir == Right) {
            return horEdges[index.value()];
        } else {
            return vertEdges[index.value()];
        }
    }
    return false;
}

bool GridGraph::setEdge(unsigned int node, Direction dir, bool value) {
    if (!validNode(node)) {
        return false;
    }

    auto index = edgeIndex(node, dir);
    if (index.has_value()) {
        if (dir == Left || dir == Right) {
            horEdges[index.value()] = value;
        } else {
            vertEdges[index.value()] = value;
        }
        return true;
    }
    return false;
}

bool GridGraph::addEdge(unsigned int node, Direction dir) {
    return setEdge(node, dir, true);
}

bool GridGraph::removeEdge(unsigned int node, Direction dir) {
    return setEdge(node, dir, false);
}

void GridGraph::getPossibleDirs(std::vector<Direction>& possibleDirs, unsigned int node) {
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

std::optional<Direction> GridGraph::addRandomEdge(const std::vector<Direction>& possibleDirs,
                                                        unsigned int node, std::mt19937& gen) {
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
            std::cerr << "Could not add edge " << dir << " from " << node << "\n";
        }
    } else {
        std::cerr << "Possible dirs empty!\n";
    }
    return {};
}
