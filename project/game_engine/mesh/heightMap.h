
#ifndef INFOH502_CPP_HEIGHTMAP_H
#define INFOH502_CPP_HEIGHTMAP_H
#include <vector>


class HeightMap {
    int _width;
    int _depth;
    int _imageWidth;
    int _imageHeight;
    std::vector<float> _heights;
public:
    HeightMap(int width, int depth, int imageWidth, int imageHeight, const std::vector<float> &heights);
    float getHeight(float x, float z) const;
};



#endif //INFOH502_CPP_HEIGHTMAP_H
