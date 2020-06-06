#include <vector>
#include <optional>
#include <random>
#include <iostream>
#include <stdexcept>

#include "mazelib/grid.hpp"

namespace mazelib {

GridGraph::GridGraph(int numRows, int numColumns) :
    numRows(numRows),
    numColumns(numColumns),
    horEdges((numColumns - 1) * numRows, false),
    vertEdges(numColumns * (numRows - 1), false) {
}

bool GridGraph::validNode(int node) const {
    return (node >= 0 && node < getNumNodes());
}

void GridGraph::validateNode(int node) const {
    if (!validNode(node)) {
        throw std::invalid_argument("Invalid node " + std::to_string(node));
    }
}

int GridGraph::getNumNodes() const {
    return numRows * numColumns;
}

int GridGraph::getNumRows() const {
    return numRows;
}

int GridGraph::getNumColumns() const {
    return numColumns;
}

int GridGraph::rowNumber(int node) const {
    return node / numColumns;
}

int GridGraph::columnNumber(int node) const {
    return node % numColumns;
}

int GridGraph::nodeNumber(int rowNum, int columnNum) const {
    return (rowNum * numColumns) + columnNum;
}

int GridGraph::nodeInDirection(int start, Direction dir) const {
    int rowNum = rowNumber(start);
    int columnNum = columnNumber(start);

    switch (dir) {
        case Left:
            if (vertexEdgeExists(rowNum, columnNum, dir)) {
                return start-1;
            }
            break;
        case Right:
            if (vertexEdgeExists(rowNum, columnNum, dir)) {
                return start+1;
            }
            break;
        case Up:
            if (vertexEdgeExists(rowNum, columnNum, dir)) {
                return nodeNumber(rowNum - 1, columnNum);
            }
            break;
        case Down:
            if (vertexEdgeExists(rowNum, columnNum, dir)) {
                return nodeNumber(rowNum + 1, columnNum);
            }
            break;
    }
    throw std::invalid_argument(
        "No edge from node " + std::to_string(start) +
        " in direction " + std::to_string(dir) + "\n"
    );
}

std::optional<int> GridGraph::edgeIndex(int node, Direction dir) const {
    int rowNum = rowNumber(node);
    int columnNum = columnNumber(node);

    switch (dir) {
        case Right:
            if (vertexEdgeExists(rowNum, columnNum, dir)) {
                return node - rowNum;
            }
            break;
        case Left:
            if (vertexEdgeExists(rowNum, columnNum, dir)) {
                return node - rowNum - 1;
            }
            break;
        case Down:
            if (vertexEdgeExists(rowNum, columnNum, dir)) {
                return node;
            }
            break;
        case Up:
            if (vertexEdgeExists(rowNum, columnNum, dir)) {
                return nodeNumber(rowNum - 1, columnNum);
            }
            break;
    }
    return {};
}

bool GridGraph::vertexEdgeExists(int rowNum, int colNum, Direction dir) const {
    bool result;

    switch (dir) {
        case Right:
            if (colNum < numColumns - 1) {
                result = true;
            } else {
                result = false;
            }
            break;
        case Left:
            if (colNum > 0) {
                result = true;
            } else {
                result = false;
            }
            break;
        case Down:
            if (rowNum < numRows - 1) {
                result = true;
            } else {
                result = false;
            }
            break;
        case Up:
            if (rowNum > 0) {
                result = true;
            } else {
                result = false;
            }
            break;
    }
    return result;
}

bool GridGraph::edgeExists(int node, Direction dir) const {
    validateNode(node);

    int rowNum = rowNumber(node);
    int columnNum = columnNumber(node);
    return vertexEdgeExists(rowNum, columnNum, dir);
}

bool GridGraph::hasEdge(int node, Direction dir) const {
    validateNode(node);

    std::optional<int> index = edgeIndex(node, dir);
    if (index.has_value()) {
        if (dir == Left || dir == Right) {
            return horEdges[index.value()];
        } else {
            return vertEdges[index.value()];
        }
    }
    return false;
}

void GridGraph::setEdge(int node, Direction dir, bool value) {
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

void GridGraph::addEdge(int node, Direction dir) {
    setEdge(node, dir, true);
}

void GridGraph::removeEdge(int node, Direction dir) {
    setEdge(node, dir, false);
}

void GridGraph::getPossibleDirs(std::vector<Direction>& possibleDirs, int node) {
    possibleDirs.clear();

    auto column = columnNumber(node);
    if (column > 0 && !hasEdge(node, Left)) {
        possibleDirs.push_back(Left);
    }
    if (column < numColumns-1 && !hasEdge(node, Right)) {
        possibleDirs.push_back(Right);
    }

    auto row = rowNumber(node);
    if (row > 0 && !hasEdge(node, Up)) {
        possibleDirs.push_back(Up);
    }
    if (row < numRows-1 && !hasEdge(node, Down)) {
        possibleDirs.push_back(Down);
    }
}

Direction GridGraph::addRandomEdge(const std::vector<Direction>& possibleDirs,
                                   int node, std::mt19937& gen) {
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
