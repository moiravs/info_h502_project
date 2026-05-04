
#include "renderableEntityMaker.h"
#include <stb_image.h>
#include "../prop/prop.h"

std::pair<std::shared_ptr<HeightMap>, std::shared_ptr<Prop>> RenderableEntityMaker::terrainFromTexture(const std::string& texturePath,
                                                                          const float width, const float depth)
{
    int imageWidth;
    int imageHeight;
    stbi_set_flip_vertically_on_load(true);
    int nrChannels;
    unsigned char *data = stbi_load(texturePath.c_str(), &imageWidth, &imageHeight, &nrChannels, 0);
    if (!data)
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    std::vector<float> heights(imageWidth * imageHeight);

    constexpr float yScale = 64.0f / 256.0f;

    auto idx = [&](const int x, const int z)
    {
        return z * imageWidth + x;
    };

    for (int z = 0; z < imageHeight; z++)
    {
        for (int x = 0; x < imageWidth; x++)
        {
            constexpr float yShift = 16.0f;
            const unsigned char *pixel = data + (x + imageWidth * z) * nrChannels;
            const float h = pixel[0] * yScale - yShift;

            heights[idx(x, z)] = h;
        }
    }

    std::vector<glm::vec3> normals(imageWidth * imageHeight);

    for (int z = 0; z < imageHeight; z++)
    {
        for (int x = 0; x < imageWidth; x++)
        {
            normals[idx(x,z)] = calculateNormal(x, z,
                                                 imageWidth,
                                                 imageHeight,
                                                 heights);
        }
    }

    auto heightMap = std::make_shared<HeightMap>(width, depth, imageWidth, imageHeight, heights);
    auto prop = std::make_shared<Prop>();

    for (int cz = 0; cz < imageHeight - 1; cz += CHUNK_SIZE)
    {
        for (int cx = 0; cx < imageWidth - 1; cx += CHUNK_SIZE)
        {
            auto mesh = std::make_shared<Mesh>();

            std::vector<Vertex> vertices;
            vertices.reserve((CHUNK_SIZE + 1) * (CHUNK_SIZE + 1));
            std::vector<unsigned> indices;
            indices.reserve(CHUNK_SIZE * CHUNK_SIZE * 6);

            int maxX = std::min(cx + CHUNK_SIZE, imageWidth - 1);
            int maxZ = std::min(cz + CHUNK_SIZE, imageHeight - 1);

            for (int z = cz; z <= maxZ; z++)
            {
                for (int x = cx; x <= maxX; x++)
                {
                    float xPercent = static_cast<float>(x) / (imageWidth - 1);
                    float zPercent = static_cast<float>(z) / (imageHeight - 1);

                    float vx = (xPercent - 0.5f) * width;
                    float vz = (zPercent - 0.5f) * depth;
                    float vy = heights[idx(x, z)];

                    glm::vec3 pos(vx, vy, vz);

                    float u = xPercent;
                    float v = zPercent;

                    glm::vec2 uv(u, v);

                    glm::vec3 n = normals[idx(x, z)];

                    vertices.push_back(Vertex{pos, uv, n});
                }
            }
            int chunkWidth = maxX - cx + 1;

            for (int z = 0; z < (maxZ - cz); z++)
            {
                for (int x = 0; x < (maxX - cx); x++)
                {
                    int i0 = z * chunkWidth + x;
                    int i1 = i0 + 1;
                    int i2 = i0 + chunkWidth;
                    int i3 = i2 + 1;

                    indices.push_back(i0);
                    indices.push_back(i2);
                    indices.push_back(i1);

                    indices.push_back(i1);
                    indices.push_back(i2);
                    indices.push_back(i3);
                }
            }
            MeshEntry entry{};
            entry.init(vertices, indices);
            entry.materialIndex = 0;
            mesh->addEntry(entry);
            std::shared_ptr<Object> object = Object::make(mesh, "cpu_height");

            prop->addEntity(object);
            prop->addRenderable(object);
            prop->setMainObject(object);
        }
    }
    stbi_image_free(data);
    return {heightMap, prop};
}

glm::vec3 RenderableEntityMaker::calculateNormal(const int x, const int z, const int imageWidth, const int imageHeight,
                                       const std::vector<float> &heights)
{
    auto idx = [&](const int _x, const int _z)
    {
        return _z * imageWidth + _x;
    };
    // Get heights of neighbors (with bounds checking)
    const float hL = (x > 0) ? heights[idx((x - 1), z)] : heights[idx(x, z)];
    const float hR = (x < imageWidth - 1) ? heights[idx(x + 1, z)] : heights[idx(x, z)];
    const float hD = (z > 0) ? heights[idx(x, z - 1)] : heights[idx(x, z)];
    const float hU = (z < imageHeight - 1) ? heights[idx(x, z + 1)] : heights[idx(x, z)];

    // Deduce normal from height difference
    // The constant (2.0) should match your grid spacing for perfect accuracy
    const glm::vec3 normal = glm::normalize(glm::vec3(hL - hR, 2.0f, hD - hU));
    return normal;
}
