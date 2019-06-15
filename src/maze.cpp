#include <random>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <deque>

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

std::optional<unsigned int> getNewNode(unsigned int numNodes,
        std::unordered_set<unsigned int> exclusions) {
    // TODO more efficient newNode
    // Get next node not in maze if there is one
    for (unsigned int n = 0; n < numNodes; ++n) {
        if (exclusions.count(n) == 0) {
            return n;
        }
    }
    return {};
}

void eraseLoop(GridGraph& graph, unsigned int loopNode,
        std::deque<Edge>& history, std::unordered_set<unsigned int>& sectionNodes) {
    // Remove edges until we reach the the node
    Edge lastEdge;
    while (!history.empty()) {
        lastEdge = history.back();
        history.pop_back();
        graph.removeEdge(lastEdge.node, lastEdge.dir);
        if (sectionNodes.count(lastEdge.node) > 0) {
            sectionNodes.erase(lastEdge.node);
        } else {
            std::cerr << "Node in history was not in section\n";
        }
        if (lastEdge.node == loopNode) {
            break;
        }
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
    std::unordered_set<unsigned int> inMaze;
    std::unordered_set<unsigned int> inSection;
    std::vector<Direction> possibleDirs;
    // TODO remove duplicate node stoarage for histroy & inSection
    std::deque<Edge> history;

    // Add starting maze node
    inMaze.insert(currentNode);

    // While there are still edges to add
    while (inMaze.size() < numNodes) {
        // If starting again
        if (inSection.empty()) {
            newNode = getNewNode(numNodes, inMaze);
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
            history.push_back(Edge{currentNode, dir.value()});
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
                if (inMaze.count(nextNode.value()) > 0) {
                    // Start new section
                    inMaze.merge(inSection);
                    inMaze.insert(nextNode.value());
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
            std::cerr << "Could not move from " << currentNode << " islen " << inSection.size() << "\n";
            break;
        }
    }
}
