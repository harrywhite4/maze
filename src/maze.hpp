#ifndef MAZE_HPP_
#define MAZE_HPP_

#include <vector>
#include <string>

#include "grid.hpp"
#include "image.hpp"

Image<bool> graphToImage(const GridGraph& graph);
std::string graphToText(const GridGraph& graph);
void lerwGraph(GridGraph& graph);

#endif  // MAZE_HPP_
