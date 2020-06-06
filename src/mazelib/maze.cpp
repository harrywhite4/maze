#include <deque>
#include <iostream>
#include <random>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

#include "bitmap/bitmap.hpp"
#include "bitmap/image.hpp"
#include "mazelib/grid.hpp"
#include "mazelib/maze.hpp"

namespace mazelib {


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

bitmap::Image<bool> graphToImage(const GridGraph& graph) {
    // Space for cells
    const int spacing = 8;
    // Thickness of cell walls
    const int thickness = 2;

    int numNodes = graph.getNumNodes();
    int numColumns = graph.getNumColumns();
    int numRows = graph.getNumRows();

    int imageRows = (numRows * spacing) + ((numRows + 1) * thickness);
    int imageColumns = (numColumns * spacing) + ((numColumns + 1) * thickness);
    bitmap::Image<bool> image(imageColumns, imageRows, false);

    // Draw top and bottom boundary
    for (int x = 0; x < imageColumns; ++x) {
        for (int t = 0; t < thickness; ++t) {
            image.setValue(x, t, true);
            image.setValue(x, imageRows - 1 - t, true);
        }
    }
    // Draw left and right boundary
    for (int y = 0; y < imageRows; ++y) {
        for (int t = 0; t < thickness; ++t) {
            image.setValue(t, y, true);
            image.setValue(imageColumns - 1 - t, y, true);
        }
    }

    // Draw inner walls
    for (int n = 0; n < numNodes; ++n) {
        int row = graph.rowNumber(n);
        int column = graph.columnNumber(n);
        int topleftX = column * (thickness + spacing);
        int topleftY = row * (thickness + spacing);
        image.setValue(topleftX, topleftY, true);
        // If not last row and there is not down edge
        if (row < numRows - 1 && !graph.hasEdge(n, Down)) {
            for (int x = 0; x < (thickness * 2 + spacing); ++x) {
                for (int y = 0; y < thickness; ++y) {
                    image.setValue(topleftX + x, topleftY + spacing + thickness + y, true);
                }
            }
        }
        // If not last column and there is no right edge
        if (column < numColumns - 1 && !graph.hasEdge(n, Right)) {
            for (int x = 0; x < thickness; ++x) {
                for (int y = 0; y < (thickness * 2 + spacing); ++y) {
                    image.setValue(topleftX + spacing + thickness + x, topleftY + y, true);
                }
            }
        }
    }
    return image;
}

std::string graphToText(const GridGraph& graph) {
    int numColumns = graph.getNumColumns();
    std::string s;
    // Add top row
    for (int i = 0; i < numColumns; ++i) {
        s.append("+--");
    }
    s.append("+\n");

    // Write rows
    for (int row = 0; row < graph.getNumRows(); ++row) {
        std::string rowTop = "|", rowBottom = "+";
        for (int column = 0; column < numColumns; ++column) {
            int nodeNum = graph.nodeNumber(row, column);
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

int getNewNode(int numNodes, std::unordered_set<int> notInSet) {
    // Get next node not in maze if there is one
    if (!notInSet.empty()) {
        return *notInSet.begin();
    }
    throw std::invalid_argument("Attempting to get new node from empty set");
}

void removeFromSet(std::unordered_set<int>& from, const std::unordered_set<int>& removals) {
    for (auto n : removals) {
        from.erase(n);
    }
}

void eraseLoop(GridGraph& graph, int loopNode,
               std::deque<Direction>& history, std::unordered_set<int>& inSection) {
    // Remove edges until we reach the the node
    int currentNode = loopNode;
    while (!history.empty()) {
        Direction lastDir = history.back();
        history.pop_back();
        Direction revDir = reverse(lastDir);
        graph.removeEdge(currentNode, revDir);

        // Remove node from section
        // except loopNode since we will exit with an edge existing to this node
        if (currentNode != loopNode) {
            inSection.erase(currentNode);
        }

        currentNode = graph.nodeInDirection(currentNode, revDir);
        if (currentNode == loopNode) {
            break;
        }
    }
    if (currentNode != loopNode) {
        std::cerr << "Loop could not be erased\n";
    }
}


void lerwGraph(GridGraph& graph) {
    int currentNode = 0;

    std::unordered_set<int> notInMaze;
    int numNodes = graph.getNumNodes();
    // Add all to not in maze except starting node
    for (int i = 0; i < numNodes; ++i) {
        if (i != currentNode) {
            notInMaze.insert(i);
        }
    }

    std::random_device rd;
    // Setup random number generator
    std::mt19937 gen(rd());
    std::unordered_set<int> inSection;
    std::vector<Direction> possibleDirs;
    std::deque<Direction> history;
    // While there are still edges to add
    while (!notInMaze.empty()) {
        // If starting again
        if (inSection.empty()) {
            int newNode = getNewNode(numNodes, notInMaze);
            currentNode = newNode;
            inSection.insert(currentNode);
        }

        // add random edge
        graph.getPossibleDirs(possibleDirs, currentNode);
        Direction dir = graph.addRandomEdge(possibleDirs, currentNode, gen);
        // Add to history
        history.push_back(dir);
        // Get node new edge goes to
        int nextNode = graph.nodeInDirection(currentNode, dir);
        if (inSection.count(nextNode) > 0) {
            // If node was already in section (we have a loop)
            eraseLoop(graph, nextNode, history, inSection);
            currentNode = nextNode;
        } else if (notInMaze.count(nextNode) == 0) {
            // If node was already in maze, start new section
            removeFromSet(notInMaze, inSection);
            inSection.clear();
        } else {
            // If node was not in section or maze
            inSection.insert(nextNode);
            currentNode = nextNode;
        }
    }
}

void dfsGraph(GridGraph& graph) {
    int currentNode = 0;
    std::unordered_set<int> visited;
    visited.insert(currentNode);

    std::random_device rd;
    // Setup random number generator
    std::mt19937 gen(rd());
    std::stack<int> nodePath;
    std::vector<Direction> possibleDirs;
    std::vector<Direction> validDirs;
    unsigned int numNodes = graph.getNumNodes();
    while (visited.size() < numNodes) {
        graph.getPossibleDirs(possibleDirs, currentNode);
        validDirs.clear();
        for (Direction dir : possibleDirs) {
            // If node in this direction is unvisited, add to valid
            if (visited.count(graph.nodeInDirection(currentNode, dir)) == 0) {
                validDirs.push_back(dir);
            }
        }
        if (!validDirs.empty()) {
            Direction lastDir = graph.addRandomEdge(validDirs, currentNode, gen);
            // Insert previous node into path
            nodePath.push(currentNode);
            // set current node add to visited
            currentNode = graph.nodeInDirection(currentNode, lastDir);
            visited.insert(currentNode);
        } else {
            if (!nodePath.empty()) {
                currentNode = nodePath.top();
                nodePath.pop();
            } else {
                throw std::out_of_range("node path is empty");
            }
        }
    }
}

void createMaze(GridGraph& graph, MazeType type) {
    switch (type) {
        case Dfs:
            dfsGraph(graph);
            break;
        case Wilsons:
            lerwGraph(graph);
            break;
    }
}

void outputMaze(const GridGraph& maze, OutputFormat format, std::string fname, bool verbose) {
    switch (format) {
        case Text:
            std::cout << graphToText(maze);
            break;
        case Bitmap:
            bitmap::Image<bool> image = graphToImage(maze);
            bitmap::writeBitmapBW(fname, image, verbose);
            break;
    }
}


} // namespace mazelib
