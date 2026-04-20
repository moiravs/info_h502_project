#ifndef TerrainGeneration_H
#define TerrainGeneration_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>

class TerrainGeneration
{
    int numStrips{};
    int numTrisPerStrip{};
    std::vector<float> vertices;
    std::vector<unsigned> indices;
    float targetWidth;
    float targetLength;
    int _width{};
    int _height{};

public:
    GLuint ID{};

    TerrainGeneration(const char *texturePath, float targetWidth, float targetLength);

    // Add this helper inside TerrainGeneration or above it
    glm::vec3 calculateNormal(int x, int z, int width, int height, const std::vector<float> &verts);

    float getHeight() const;

    float getWidth() const;

    float getHeight(float worldX, float worldZ) const;

    std::vector<float> &getVertices();

    std::vector<unsigned> &getIndices();
};
#endif