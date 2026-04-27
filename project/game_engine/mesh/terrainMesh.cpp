
#include "terrainMesh.h"
#include <stb_image.h>

TerrainMesh::TerrainMesh(const float width, const float depth): Mesh(), _width(width), _depth(depth)
{}

std::shared_ptr<TerrainMesh> TerrainMesh::terrainFromTexture(const std::string& texturePath,
    const float width, const float depth)
{
    auto ret = std::make_shared<TerrainMesh>(width, depth);

    std::vector<unsigned> indices {};
    stbi_set_flip_vertically_on_load(true);
    int nrChannels;
    unsigned char *data = stbi_load(texturePath.c_str(), &ret->_imageWidth, &ret->_imageHeight, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    std::vector<float> heights(ret->_imageWidth * ret->_imageHeight);

    constexpr float yScale = 64.0f / 256.0f;

    auto idx = [&](const int x, const int z)
    {
        return z * ret->_imageWidth + x;
    };

    for (int z = 0; z < ret->_imageHeight; z++)
    {
        for (int x = 0; x < ret->_imageWidth; x++)
        {
            constexpr float yShift = 16.0f;
            const unsigned char* pixel = data + (x + ret->_imageWidth * z) * nrChannels;
            const float h = pixel[0] * yScale - yShift;

            heights[idx(x, z)] = h;
        }
    }

    for (int z = 0; z < ret->_imageHeight; z++)
    {
        for (int x = 0; x < ret->_imageWidth; x++)
        {
            float xPercent = static_cast<float>(x) / static_cast<float>(ret->_imageWidth - 1);
            float zPercent = static_cast<float>(z) / static_cast<float>(ret->_imageHeight - 1);

            float vx = (xPercent - 0.5f) * width;
            float vz = (zPercent - 0.5f) * depth;
            float vy = heights[idx(x, z)];

            glm::vec3 pos(vx, vy, vz);

            float u = static_cast<float>(x) / (ret->_imageWidth - 1);
            float v = static_cast<float>(z) / (ret->_imageHeight - 1);

            glm::vec2 uv(u, v);

            glm::vec3 n = calculateNormal(x, z, ret->_imageWidth, ret->_imageHeight, heights);

            ret->_vertices.push_back(Vertex{pos, uv, n});
        }
    }

    for (int z = 0; z < ret->_imageHeight - 1; z++)
    {
        for (int x = 0; x < ret->_imageWidth - 1; x++)
        {
            int i0 = idx(x, z);
            int i1 = idx(x + 1, z);
            int i2 = idx(x, z + 1);
            int i3 = idx(x + 1, z + 1);

            // triangle 1
            indices.push_back(i0);
            indices.push_back(i2);
            indices.push_back(i1);

            // triangle 2
            indices.push_back(i1);
            indices.push_back(i2);
            indices.push_back(i3);
        }
    }

    stbi_image_free(data);

    MeshEntry entry{};
    entry.init(ret->_vertices, indices);
    entry.materialIndex = 0;
    ret->m_Entries.push_back(entry);

    return ret;
}

glm::vec3 TerrainMesh::calculateNormal(const int x, const int z, const int imageWidth, const int imageHeight,
    const std::vector<float>& heights)
{
    auto idx = [&](const int _x, const int _z)
    {
        return _z * imageWidth + _x;
    };
    // Get heights of neighbors (with bounds checking)
    const float hL = (x > 0) ? heights[idx((x - 1), z)] : heights[idx(x, z)];
    const float hR = (x < imageHeight - 1) ? heights[idx(x + 1, z)] : heights[idx(x, z)];
    const float hD = (z > 0) ? heights[idx(x, z - 1)] : heights[idx(x, z)];
    const float hU = (z < imageWidth - 1) ? heights[idx(x, z + 1)] : heights[idx(x, z)];

    // Deduce normal from height difference
    // The constant (2.0) should match your grid spacing for perfect accuracy
    const glm::vec3 normal = glm::normalize(glm::vec3(hL - hR, 2.0f, hD - hU));
    return normal;
}

float TerrainMesh::getHeight(const float x, const float z) const
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

    return this->getVertices().at(row * _imageWidth + col).position.y;
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
