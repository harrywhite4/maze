#include <vector>

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
