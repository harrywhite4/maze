#ifndef MAZELIB_MAZE_HPP_
#define MAZELIB_MAZE_HPP_

#include <vector>
#include <string>

#include "grid.hpp"
#include "image.hpp"

Image<bool> graphToImage(const GridGraph& graph);
std::string graphToText(const GridGraph& graph);
void lerwGraph(GridGraph& graph);

#endif  // MAZELIB_MAZE_HPP_
