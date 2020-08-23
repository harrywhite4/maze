#ifndef MAZELIB_MAZE_HPP_
#define MAZELIB_MAZE_HPP_

#include <map>
#include <string>
#include <vector>

#include "bitmap/image.hpp"
#include "mazelib/grid.hpp"

namespace mazelib {

enum MazeType {Dfs, Wilsons};
std::ostream& operator<<(std::ostream& os, const MazeType& mt);
std::istream& operator>>(std::istream& is, MazeType& mt);

enum OutputFormat {Bitmap, Text};
std::ostream& operator<<(std::ostream& os, const OutputFormat& of);
std::istream& operator>>(std::istream& is, OutputFormat& of);

void createMaze(GridGraph& graph, MazeType type);
void outputMaze(const GridGraph& maze, OutputFormat format, std::string fname, bool verbose);
bitmap::Image<bool> graphToImage(const GridGraph& graph);
std::string graphToText(const GridGraph& graph);
void lerwGraph(GridGraph& graph);
void dfsGraph(GridGraph& graph);

} // namespace mazelib

#endif  // MAZELIB_MAZE_HPP_
