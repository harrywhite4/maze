#ifndef MAZELIB_ERROR_HPP_
#define MAZELIB_ERROR_HPP_

#include <exception>
#include <string>

namespace mazelib {

class MazeError : public std::exception {
    std::string message;

 public:
    explicit MazeError(std::string message) : message(message) {
    }

    const char* what() const noexcept override {
        return message.c_str();
    }
};

} // namespace mazelib

#endif  // MAZELIB_ERROR_HPP_
