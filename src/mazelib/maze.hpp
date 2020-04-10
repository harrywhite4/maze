#ifndef MAZELIB_MAZE_HPP_
#define MAZELIB_MAZE_HPP_

#include <vector>
#include <string>

#include "mazelib/grid.hpp"
#include "bitmap/image.hpp"

namespace mazelib {

bitmap::Image<bool> graphToImage(const GridGraph& graph);
std::string graphToText(const GridGraph& graph);
void lerwGraph(GridGraph& graph);

} // namespace mazelib

#endif  // MAZELIB_MAZE_HPP_
