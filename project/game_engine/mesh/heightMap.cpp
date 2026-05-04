
#include "heightMap.h"

HeightMap::HeightMap(const int width, const int depth, const int imageWidth, const int imageHeight,
    const std::vector<float> &heights)
    : _width(width), _depth(depth), _imageWidth(imageWidth), _imageHeight(imageHeight), _heights(heights) {  }

float HeightMap::getHeight(const float x, const float z) const
{
    const float xPercent = (x / this->_width) + 0.5f;
    const float zPercent = (z / this->_depth) + 0.5f;

    int col = static_cast<int>(xPercent * (static_cast<float>(_imageWidth) - 1));
    int row = static_cast<int>(zPercent * (static_cast<float>(_imageHeight) - 1));

    if (col < 0)
        col = 0;
    if (col >= _imageWidth)
        col = _imageWidth - 1;
    if (row < 0)
        row = 0;
    if (row >= _imageHeight)
        row = _imageHeight - 1;

    return this->_heights.at(row * _imageWidth + col);
}