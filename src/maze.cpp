#include <vector>

#include "maze.h"
#include "grid.h"

std::vector<std::vector<bool>> graphToImage(GridGraph& graph) {
    unsigned int pixelX, pixelY, row, column;
    unsigned int numNodes = graph.getNumNodes();
    unsigned int imageRows = (graph.getNumRows() * 2) + 1;
    unsigned int imageColumns = (graph.getNumColumns() * 2) + 1;
    std::vector<std::vector<bool>> image(imageRows, std::vector<bool>(imageColumns, false));
    for (unsigned int n = 0; n < numNodes; ++n) {
        row = graph.rowNumber(n);
        column = graph.columnNumber(n);
        pixelX = (row * 2) + 1;
        pixelY = (column * 2) + 1;
        if (!graph.hasEdge(n, Right)) {
            image[pixelX + 1][pixelY] = true;
        }
        if (!graph.hasEdge(n, Down)) {
            image[pixelX][pixelY + 1] = true;
        }
    }
    return image;
}
