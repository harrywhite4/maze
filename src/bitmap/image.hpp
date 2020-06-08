#ifndef BITMAP_IMAGE_HPP_
#define BITMAP_IMAGE_HPP_

#include <vector>

namespace bitmap {

// Class for storing image data
template <typename T>
class Image {
    int numColumns;
    std::vector<T> data;
    int getArrayIndex(int x, int y);

 public:
    Image(int numColumns, int numRows);
    Image(int numColumns, int numRows, T initial);
    int getNumColumns();
    int getNumRows();
    T getValue(int x, int y);
    void setValue(int x, int y, T value);
};

template <typename T>
Image<T>::Image(int numColumns, int numRows) :
    numColumns(numColumns),
    data(numColumns * numRows) {
}

template <typename T>
Image<T>::Image(int numColumns, int numRows, T initial) :
    numColumns(numColumns),
    data(numColumns * numRows, initial) {
}

template <typename T>
int Image<T>::getArrayIndex(int x, int y) {
    return (y * numColumns) + x;
}

template <typename T>
int Image<T>::getNumColumns() {
    return numColumns;
}

template <typename T>
int Image<T>::getNumRows() {
    return data.size() / numColumns;
}

template <typename T>
T Image<T>::getValue(int x, int y) {
    return data[getArrayIndex(x, y)];
}

template <typename T>
void Image<T>::setValue(int x, int y, T value) {
    data[getArrayIndex(x, y)] = value;
}

} // namespace bitmap

#endif  // BITMAP_IMAGE_HPP_
