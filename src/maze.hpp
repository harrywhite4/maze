#ifndef MAZE_HPP_
#define MAZE_HPP_

#include <vector>
#include "grid.hpp"

std::vector<std::vector<bool>> graphToImage(const GridGraph& graph);
GridGraph lerwGraph(int numRown, int numColumns);

#endif  // MAZE_HPP_
