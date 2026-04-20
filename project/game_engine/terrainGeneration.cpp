#include "terrainGeneration.h"

#include <stb_image.h>

TerrainGeneration::TerrainGeneration(const char* texturePath, const float targetWidth, const float targetLength)
{
    this->targetWidth = targetWidth;
    this->targetLength = targetLength;
    stbi_set_flip_vertically_on_load(true);
    int nrChannels;
    unsigned char *data = stbi_load(texturePath, &_width, &_height, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    constexpr float yScale = 64.0f / 256.0f;
    constexpr int rez = 1;
    const unsigned bytePerPixel = nrChannels;
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            constexpr float yShift = 16.0f;
            const unsigned char *pixelOffset = data + (j + _width * i) * bytePerPixel;
            const unsigned char y = pixelOffset[0];

            // 1. Normalize the loop index to 0.0 -> 1.0
            const float xPercent = static_cast<float>(i) / (_height - 1);
            const float zPercent = static_cast<float>(j) / (_width - 1);

            // 2. Multiply by target dimensions and center it
            float vx = (xPercent - 0.5f) * targetLength;
            float vy = static_cast<int>(y) * yScale - yShift;
            float vz = (zPercent - 0.5f) * targetWidth;

            vertices.push_back(vx);
            vertices.push_back(vy);
            vertices.push_back(vz);
        }
    }

    stbi_image_free(data);

    for (unsigned i = 0; i < _height - 1; i += rez)
    {
        for (unsigned j = 0; j < _width; j += rez)
        {
            for (unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + _width * (i + k * rez));
            }
        }
    }

    // Inside your constructor, after the first loop that fills 'vertices':
    std::vector<float> verticesWithNormals;
    for (int i = 0; i < _height; i++)
    {
        for (int j = 0; j < _width; j++)
        {
            // Add Position
            int idx = (i * _width + j) * 3;
            verticesWithNormals.push_back(vertices[idx]);
            verticesWithNormals.push_back(vertices[idx + 1]);
            verticesWithNormals.push_back(vertices[idx + 2]);

            // Add Normal
            glm::vec3 n = calculateNormal(i, j, _width, _height, vertices);
            verticesWithNormals.push_back(n.x);
            verticesWithNormals.push_back(n.y);
            verticesWithNormals.push_back(n.z);
        }
    }
    vertices = verticesWithNormals; // Replace old vector
}

glm::vec3 TerrainGeneration::calculateNormal(const int x, const int z, const int width, const int height, const std::vector<float>& verts)
{
    // Get heights of neighbors (with bounds checking)
    const float hL = (x > 0) ? verts[((x - 1) * width + z) * 3 + 1] : verts[(x * width + z) * 3 + 1];
    const float hR = (x < height - 1) ? verts[((x + 1) * width + z) * 3 + 1] : verts[(x * width + z) * 3 + 1];
    const float hD = (z > 0) ? verts[(x * width + (z - 1)) * 3 + 1] : verts[(x * width + z) * 3 + 1];
    const float hU = (z < width - 1) ? verts[(x * width + (z + 1)) * 3 + 1] : verts[(x * width + z) * 3 + 1];

    // Deduce normal from height difference
    // The constant (2.0) should match your grid spacing for perfect accuracy
    glm::vec3 normal = glm::normalize(glm::vec3(hL - hR, 2.0f, hD - hU));
    return normal;
}

float TerrainGeneration::getHeight() const
{
    return this->_height;
}

float TerrainGeneration::getWidth() const
{
    return this->_width;
}

float TerrainGeneration::getHeight(const float worldX, const float worldZ) const
{
    const float xPercent = (worldX / targetLength) + 0.5f;
    const float zPercent = (worldZ / targetWidth) + 0.5f;

    int col = static_cast<int>(xPercent * (_height - 1));
    int row = static_cast<int>(zPercent * (_width - 1));

    if (col < 0)
        col = 0;
    if (col >= _height)
        col = _height - 1;
    if (row < 0)
        row = 0;
    if (row >= _width)
        row = _width - 1;

    const int vertexIndex = (col * _width + row) * 6;
    return vertices[vertexIndex + 1];
}

std::vector<float>& TerrainGeneration::getVertices()
{
    return vertices;
}

std::vector<unsigned>& TerrainGeneration::getIndices()
{
    return indices;
}
