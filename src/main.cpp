#include <string>
#include <vector>
#include <iostream>

#include "bitmap.hpp"
#include "grid.hpp"
#include "maze.hpp"

const char usage[] = "Usage: bitmap [options] fname\nOptions:\n"
                          "--help Print help\n";

int main(int argc, char *argv[]) {
    // Variables
    bool success;
    // Argument parsing
    std::vector<std::string> posArgs, flagArgs;
    std::string current;
    for (int i = 1; i < argc; ++i) {
        current = argv[i];
        if (current[0] == '-') {
            flagArgs.push_back(current);
        } else {
            posArgs.push_back(current);
        }
    }

    // Set based on flagArgs
    for (auto flag : flagArgs) {
        if (flag == "--help") {
            std::cout << usage;
            exit(EXIT_SUCCESS);
        } else {
            std::cerr << "Invalid argument " << flag << "\n" << usage;
            exit(EXIT_FAILURE);
        }
    }

    // Set based on posArgs
    if (posArgs.size() != 1) {
        std::cerr << "Filename positional argument required\n" << usage;
        exit(EXIT_FAILURE);
    }
    std::string fname = posArgs[0];

    // Build data
    GridGraph graph(3, 3);
    graph.addEdge(0, Right);
    graph.addEdge(0, Down);
    graph.addEdge(1, Right);
    graph.addEdge(3, Right);
    graph.addEdge(4, Right);
    graph.addEdge(5, Down);
    graph.addEdge(6, Right);
    graph.addEdge(7, Right);
    auto image = graphToImage(graph);
    success = writeBitmapBW(fname, image);

    if (!success) {
        std::cout << "Could not write to file\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Done!\n";
    exit(EXIT_SUCCESS);
}
