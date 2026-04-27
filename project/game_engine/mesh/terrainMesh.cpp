
#include "terrainMesh.h"
#include <stb_image.h>

TerrainMesh::TerrainMesh(const float width, const float depth): Mesh(), _width(width), _depth(depth)
{}

std::shared_ptr<TerrainMesh> TerrainMesh::terrainFromTexture(const std::string& texturePath,
    const float width, const float depth)
{
    auto ret = std::make_shared<TerrainMesh>(width, depth);

    std::vector<float> verticesPosition {};
    std::vector<unsigned> indices {};
    stbi_set_flip_vertically_on_load(true);
    int nrChannels, imageWidth, imageHeight;
    unsigned char *data = stbi_load(texturePath.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    constexpr float yScale = 64.0f / 256.0f;
    constexpr int rez = 1;
    const unsigned bytePerPixel = nrChannels;
    for (int i = 0; i < imageHeight; i++)
    {
        for (int j = 0; j < imageWidth; j++)
        {
            constexpr float yShift = 16.0f;
            const unsigned char *pixelOffset = data + (j + imageWidth * i) * bytePerPixel;
            const unsigned char y = pixelOffset[0];

            // 1. Normalize the loop index to 0.0 -> 1.0
            const float xPercent = static_cast<float>(i) / (imageHeight - 1);
            const float zPercent = static_cast<float>(j) / (imageWidth - 1);

            // 2. Multiply by target dimensions and center it
            verticesPosition.push_back((xPercent - 0.5f) * depth);
            verticesPosition.push_back(static_cast<int>(y) * yScale - yShift);
            verticesPosition.push_back((zPercent - 0.5f) * width);

            for (unsigned k = 0; k < 2; k++)
            {
                indices.push_back(j + imageWidth * (i + k * rez));
            }
        }
    }

    for (int i = 0; i < imageHeight; i++)
    {
        for (int j = 0; j < imageWidth; j++)
        {
            // Add Position
            const int idx = (i * imageWidth + j) * 3;
            const glm::vec3 position = {verticesPosition[idx], verticesPosition[idx + 1], verticesPosition[idx + 2]};

            // Add Normal
            const glm::vec3 n = calculateNormal(i, j, imageWidth, imageHeight, verticesPosition);

            ret->_vertices.push_back(Vertex{position, glm::vec2(0), n});
        }
    }

    stbi_image_free(data);

    MeshEntry entry {};
    entry.init(ret->_vertices, indices);

    ret->m_Entries.push_back(entry);

    return ret;
}

glm::vec3 TerrainMesh::calculateNormal(const int x, const int z, const int imageWidth, const int imageHeight,
    const std::vector<float>& vertices)
{
    // Get heights of neighbors (with bounds checking)
    const float hL = (x > 0) ? vertices[((x - 1) * imageWidth + z) * 3 + 1] : vertices[(x * imageWidth + z) * 3 + 1];
    const float hR = (x < imageHeight - 1) ? vertices[((x + 1) * imageWidth + z) * 3 + 1] : vertices[(x * imageWidth + z) * 3 + 1];
    const float hD = (z > 0) ? vertices[(x * imageWidth + (z - 1)) * 3 + 1] : vertices[(x * imageWidth + z) * 3 + 1];
    const float hU = (z < imageWidth - 1) ? vertices[(x * imageWidth + (z + 1)) * 3 + 1] : vertices[(x * imageWidth + z) * 3 + 1];

    // Deduce normal from height difference
    // The constant (2.0) should match your grid spacing for perfect accuracy
    const glm::vec3 normal = glm::normalize(glm::vec3(hL - hR, 2.0f, hD - hU));
    return normal;
}

float TerrainMesh::getHeight(const float x, const float z) const
{
    const float xPercent = (x / this->_depth) + 0.5f;
    const float zPercent = (z / this->_width) + 0.5f;

    int col = static_cast<int>(xPercent * (_depth - 1));
    int row = static_cast<int>(zPercent * (_width - 1));

    if (col < 0)
        col = 0;
    if (col >= _depth)
        col = _depth - 1;
    if (row < 0)
        row = 0;
    if (row >= _width)
        row = _width - 1;

    return this->getVertices()[col * _width + row].position.y;
}

float TerrainMesh::getDepth() const
{
    return this->_depth;
}

float TerrainMesh::getWidth() const
{
    return this->_width;
}

const std::vector<Vertex>& TerrainMesh::getVertices() const
{
    return this->_vertices;
}
