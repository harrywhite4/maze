#include <random>
#include <unordered_set>
#include <vector>
#include <iostream>

#include "maze.hpp"
#include "grid.hpp"

std::vector<std::vector<bool>> graphToImage(const GridGraph& graph) {
    unsigned int pixelX, pixelY, row, column;
    unsigned int numNodes = graph.getNumNodes();
    unsigned int imageRows = (graph.getNumRows() * 2) + 1;
    unsigned int imageColumns = (graph.getNumColumns() * 2) + 1;
    std::vector<std::vector<bool>> image(imageRows, std::vector<bool>(imageColumns, true));
    for (unsigned int n = 0; n < numNodes; ++n) {
        row = graph.rowNumber(n);
        column = graph.columnNumber(n);
        pixelY = (row * 2) + 1;
        pixelX = (column * 2) + 1;
        image[pixelY][pixelX] = false;
        if (graph.hasEdge(n, Right)) {
            image[pixelY][pixelX + 1] = false;
        }
        if (graph.hasEdge(n, Down)) {
            image[pixelY + 1][pixelX] = false;
        }
    }
    return image;
}

GridGraph lerwGraph(int numRows, int numColumns) {
    // Setup random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    // Setup data structures
    GridGraph graph = GridGraph(numRows, numColumns);
    std::unordered_set<unsigned int> inMaze;
    std::unordered_set<unsigned int> inSection;
    // Variables
    bool newNode = false;
    unsigned int currentNode = 0;
    std::optional<unsigned int> nextNode;
    std::optional<Direction> dir;

    inSection.insert(currentNode);
    auto numNodes = graph.getNumNodes();
    // While there are still edges to add
    while (inMaze.size() < numNodes) {
        // If starting again
        if (inSection.empty()) {
            newNode = false;
            // Get next node not in maze if there is one
            for (unsigned int n = 0; n < numNodes; ++n) {
                if (inMaze.count(n) == 0) {
                    currentNode = n;
                    newNode = true;
                }
            }
            if (newNode) {
                inSection.insert(currentNode);
            } else {
                std::cout << "Could not reset\n";
                break;
            }
        }

        // add random edge
        dir = graph.addRandomEdgeWithExclusions(currentNode, gen, inSection);
        if (dir.has_value()) {
            // Get node new edge goes to
            nextNode = graph.nodeInDirection(currentNode, dir.value());
            if (nextNode.has_value()) {
                // If node was already in section (we have a loop)
                if (inSection.count(nextNode.value()) > 0) {
                    std::cout << "We looped!\n";
                    break;
                }
                if (inMaze.count(nextNode.value()) > 0) {
                    // If node was already in maze
                    // Start new section
                    std::cout << "Starting new section\n";
                    inMaze.merge(inSection);
                    inMaze.insert(nextNode.value());
                    inSection.clear();
                } else {
                    // If node was not in section or maze
                    inSection.insert(nextNode.value());
                    currentNode = nextNode.value();
                }
            } else {
                std::cout << "Could not get next node!\n";
                break;
            }
        } else {
            // If can't add edge from currentNode
            std::cout << "Could not move from " << currentNode << " islen " << inSection.size() << "\n";
            inMaze.merge(inSection);
            inSection.clear();
        }
    }
    return graph;
}
