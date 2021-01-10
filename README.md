# maze

Command line tool to generate mazes, ouputs as bitmap or text

![Example output](https://i.imgur.com/NZ36gsD.png)

## Building

Follow these steps to build the project

1. Install cmake and a c++17 compiler
1. Clone repo with `git clone --recursive` to clone with submodules. If you have already cloned you
   can run `git submodule update --init --recursive`
1. Create build directory with `mkdir build`
1. Generate build system with `cmake -B build -S .`
1. Run the build with `cmake --build build`

After this the main executable can be found at `./build/bin/maze`

## Testing

After a build run `ctest` from the build directory to run the tests.
