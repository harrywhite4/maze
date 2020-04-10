#ifndef BITMAP_IMAGE_HPP_
#define BITMAP_IMAGE_HPP_

#include <vector>

namespace bitmap {

// Class for storing image data
template <typename T>
class Image {
    unsigned int numColumns;
    std::vector<T> data;
    unsigned int getArrayIndex(unsigned int x, unsigned int y);

 public:
    Image(unsigned int numColumns, unsigned int numRows);
    Image(unsigned int numColumns, unsigned int numRows, T initial);
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
Image<T>::Image(unsigned int numColumns, unsigned int numRows, T initial) :
    numColumns(numColumns),
    data(numColumns * numRows, initial) {
}

template <typename T>
unsigned int Image<T>::getArrayIndex(unsigned int x, unsigned int y) {
    return (y * numColumns) + x;
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

} // namespace bitmap

#endif  // BITMAP_IMAGE_HPP_
