#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <vector>

// Class for storing image data
template <typename T>
class Image {
    std::vector<T> data;
    unsigned int numColumns;
    unsigned int getArrayIndex(unsigned int x, unsigned int y);

 public:
    Image(unsigned int numColumns, unsigned int numRows);
    unsigned int getNumColumns();
    unsigned int getNumRows();
    T getValue(unsigned int x, unsigned int y);
    void setValue(unsigned int x, unsigned int y, T value);
};

template <typename T>
Image<T>::Image(unsigned int numColumns, unsigned int numRows) :
    numColumns(numColumns),
    data(numColumns * numRows) {
}

template <typename T>
unsigned int Image<T>::getArrayIndex(unsigned int x, unsigned int y) {
    return (y * numColumns) - 1 + x;
}

template <typename T>
unsigned int Image<T>::getNumColumns() {
    return numColumns;
}

template <typename T>
unsigned int Image<T>::getNumRows() {
    return data.size() / numColumns;
}

template <typename T>
T Image<T>::getValue(unsigned int x, unsigned int y) {
    return data[getArrayIndex(x, y)];
}

template <typename T>
void Image<T>::setValue(unsigned int x, unsigned int y, T value) {
    data[getArrayIndex(x, y)] = value;
}

#endif  // IMAGE_HPP_
