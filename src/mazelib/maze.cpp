#include <random>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <deque>
#include <string>

#include "mazelib/maze.hpp"
#include "mazelib/grid.hpp"
#include "bitmap/image.hpp"

Direction reverse(Direction dir) {
    Direction reversed;
    switch (dir) {
        case Left:
            reversed = Right;
            break;
        case Right:
            reversed = Left;
            break;
        case Up:
            reversed = Down;
            break;
        case Down:
            reversed = Up;
            break;
    }
    return reversed;
}

Image<bool> graphToImage(const GridGraph& graph) {
    unsigned int pixelX, pixelY, row, column;
    unsigned int numNodes = graph.getNumNodes();

    unsigned int imageRows = (graph.getNumRows() * 2) + 1;
    unsigned int imageColumns = (graph.getNumColumns() * 2) + 1;
    Image<bool> image(imageColumns, imageRows, true);

    for (unsigned int n = 0; n < numNodes; ++n) {
        row = graph.rowNumber(n);
        column = graph.columnNumber(n);
        pixelY = (row * 2) + 1;
        pixelX = (column * 2) + 1;
        image.setValue(pixelX, pixelY, false);
        if (graph.hasEdge(n, Right)) {
            image.setValue(pixelX+1, pixelY, false);
        }
        if (graph.hasEdge(n, Down)) {
            image.setValue(pixelX, pixelY+1, false);
        }
    }
    return image;
}

std::string graphToText(const GridGraph& graph) {
    std::string s;
    unsigned int numRows = graph.getNumRows();
    unsigned int numColumns = graph.getNumColumns();

    // Add top row
    for (unsigned int i = 0; i < numColumns; ++i) {
        s.append("+--");
    }
    s.append("+\n");

    // Write rows
    for (unsigned int row = 0; row < numRows; ++row) {
        std::string rowTop = "|", rowBottom = "+";
        for (unsigned int column = 0; column < numColumns; ++column) {
            unsigned int nodeNum = graph.nodeNumber(row, column);
            // Add to top
            if (graph.hasEdge(nodeNum, Right)) {
                rowTop.append("   ");
            } else {
                rowTop.append("  |");
            }
            // Add to bottom
            if (graph.hasEdge(nodeNum, Down)) {
                rowBottom.append("  +");
            } else {
                rowBottom.append("--+");
            }
        }
        // Add to overall string
        rowTop.push_back('\n');
        rowBottom.push_back('\n');
        s.append(rowTop);
        s.append(rowBottom);
    }

    return s;
}

std::optional<unsigned int> getNewNode(unsigned int numNodes,
                                       std::unordered_set<unsigned int> notInSet) {
    // Get next node not in maze if there is one
    if (!notInSet.empty()) {
        return *notInSet.begin();
    }
    return {};
}

void removeFromSet(std::unordered_set<unsigned int>& from,
                   const std::unordered_set<unsigned int>& removals) {
    for (auto n : removals) {
        from.erase(n);
    }
}

void eraseLoop(GridGraph& graph, unsigned int loopNode,
               std::deque<Direction>& history, std::unordered_set<unsigned int>& inSection) {
    // Remove edges until we reach the the node
    unsigned int currentNode = loopNode;
    Direction lastDir, revDir;
    bool removed;
    while (!history.empty()) {
        lastDir = history.back();
        history.pop_back();

        revDir = reverse(lastDir);
        removed = graph.removeEdge(currentNode, revDir);
        if (!removed) {
            std::cerr << "Edge could not be removed\n";
        }
        inSection.erase(currentNode);

        currentNode = graph.nodeInDirection(currentNode, revDir).value();
        if (currentNode == loopNode) {
            break;
        }
    }
    if (currentNode != loopNode) {
        std::cerr << "Loop could not be erased\n";
    }
}


void lerwGraph(GridGraph& graph) {
    // Setup random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    // Variables
    unsigned int currentNode = 0;
    unsigned int numNodes = graph.getNumNodes();
    std::optional<unsigned int> nextNode, newNode;
    std::optional<Direction> dir;
    // Setup data structures
    std::unordered_set<unsigned int> notInMaze;
    std::unordered_set<unsigned int> inSection;
    std::vector<Direction> possibleDirs;
    std::deque<Direction> history;

    // Add all to not in maze except starting node
    for (unsigned int i = 0; i < numNodes; ++i) {
        if (i != currentNode) {
            notInMaze.insert(i);
        }
    }

    // While there are still edges to add
    while (!notInMaze.empty()) {
        // If starting again
        if (inSection.empty()) {
            newNode = getNewNode(numNodes, notInMaze);
            if (newNode.has_value()) {
                currentNode = newNode.value();
                inSection.insert(currentNode);
            } else {
                std::cerr << "Could not reset\n";
                break;
            }
        }

        // add random edge
        graph.getPossibleDirs(possibleDirs, currentNode);
        dir = graph.addRandomEdge(possibleDirs, currentNode, gen);
        if (dir.has_value()) {
            // Add to history
            history.push_back(dir.value());
            // Get node new edge goes to
            nextNode = graph.nodeInDirection(currentNode, dir.value());
            if (nextNode.has_value()) {
                // If node was already in section (we have a loop)
                if (inSection.count(nextNode.value()) > 0) {
                    // Erase loop
                    eraseLoop(graph, nextNode.value(), history, inSection);
                    currentNode = nextNode.value();
                    // Add node back to section
                    inSection.insert(currentNode);
                    continue;
                }
                // If node was already in maze
                if (notInMaze.count(nextNode.value()) == 0) {
                    // Start new section
                    removeFromSet(notInMaze, inSection);
                    inSection.clear();
                } else {
                    // If node was not in section or maze
                    inSection.insert(nextNode.value());
                    currentNode = nextNode.value();
                }
            } else {
                std::cerr << "Could not get next node!\n";
                break;
            }
        } else {
            // If can't add edge from currentNode
            std::cerr << "Could not move from " << currentNode
                << " islen " << inSection.size() << "\n";
            break;
        }
    }
}
