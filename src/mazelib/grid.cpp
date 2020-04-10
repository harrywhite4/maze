#include <vector>
#include <optional>
#include <random>
#include <iostream>
#include <stdexcept>

#include "mazelib/grid.hpp"

namespace mazelib {

GridGraph::GridGraph(unsigned int numRows, unsigned int numColumns) :
    numRows(numRows),
    numColumns(numColumns),
    horEdges((numColumns - 1) * numRows, false),
    vertEdges(numColumns * (numRows - 1), false) {
}

bool GridGraph::validNode(unsigned int node) const {
    return (node < getNumNodes());
}

void GridGraph::validateNode(unsigned int node) const {
    if (!validNode(node)) {
        throw std::invalid_argument("Invalid node " + std::to_string(node));
    }
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

unsigned int GridGraph::nodeInDirection(unsigned int start, Direction dir) const {
    validateNode(start);

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
    throw std::invalid_argument(
        "No edge from node " + std::to_string(start) +
        " in direction " + std::to_string(dir) + "\n"
    );
}

std::optional<unsigned int> GridGraph::edgeIndex(unsigned int node, Direction dir) const {
    validateNode(node);

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
    return {};
}

bool GridGraph::hasEdge(unsigned int node, Direction dir) const {
    validateNode(node);

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

void GridGraph::setEdge(unsigned int node, Direction dir, bool value) {
    validateNode(node);

    auto index = edgeIndex(node, dir);
    if (index.has_value()) {
        if (dir == Left || dir == Right) {
            horEdges[index.value()] = value;
        } else {
            vertEdges[index.value()] = value;
        }
    } else {
        throw std::invalid_argument(
            "No edge from node " + std::to_string(node) +
            " in direction " + std::to_string(dir) + "\n"
        );
    }
}

void GridGraph::addEdge(unsigned int node, Direction dir) {
    setEdge(node, dir, true);
}

void GridGraph::removeEdge(unsigned int node, Direction dir) {
    setEdge(node, dir, false);
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

Direction GridGraph::addRandomEdge(const std::vector<Direction>& possibleDirs,
                                   unsigned int node, std::mt19937& gen) {
    if (possibleDirs.empty()) {
        throw std::invalid_argument("Trying to add random edge with no possible directions");
    }

    auto size = possibleDirs.size();
    auto dir = possibleDirs[0];
    // Choose random if there are more than 1
    if (size > 1) {
        std::uniform_int_distribution<> dis(0, possibleDirs.size()-1);
        dir = possibleDirs[dis(gen)];
    }
    addEdge(node, dir);
    return dir;
}

} // namespace mazelib
