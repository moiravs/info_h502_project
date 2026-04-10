#ifndef TerrainGeneration_H
#define TerrainGeneration_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "texture.h"

class TerrainGeneration
{
    int numStrips;
    int numTrisPerStrip;
    std::vector<float> vertices;
    std::vector<unsigned> indices;
    float targetWidth;
    float targetLength;
    int width;
    int height;

public:
    GLuint ID;

    TerrainGeneration(const char *texturePath, float targetWidth, float targetLength)
    {

        this->targetWidth = targetWidth;
        this->targetLength = targetLength;
        stbi_set_flip_vertically_on_load(true);
        int nrChannels;
        unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
        if (!data)
        {
            std::cout << "Failed to load texture" << std::endl;
        }

        float yScale = 64.0f / 256.0f, yShift = 16.0f;
        int rez = 1;
        unsigned bytePerPixel = nrChannels;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                unsigned char *pixelOffset = data + (j + width * i) * bytePerPixel;
                unsigned char y = pixelOffset[0];

                // 1. Normalize the loop index to 0.0 -> 1.0
                float xPercent = (float)i / (height - 1);
                float zPercent = (float)j / (width - 1);

                // 2. Multiply by target dimensions and center it
                float vx = (xPercent - 0.5f) * targetLength;
                float vy = (int)y * yScale - yShift;
                float vz = (zPercent - 0.5f) * targetWidth;

                vertices.push_back(vx);
                vertices.push_back(vy);
                vertices.push_back(vz);
            }
        }

        stbi_image_free(data);

        for (unsigned i = 0; i < height - 1; i += rez)
        {
            for (unsigned j = 0; j < width; j += rez)
            {
                for (unsigned k = 0; k < 2; k++)
                {
                    indices.push_back(j + width * (i + k * rez));
                }
            }
        }

        // Inside your constructor, after the first loop that fills 'vertices':
        std::vector<float> verticesWithNormals;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                // Add Position
                int idx = (i * width + j) * 3;
                verticesWithNormals.push_back(vertices[idx]);
                verticesWithNormals.push_back(vertices[idx + 1]);
                verticesWithNormals.push_back(vertices[idx + 2]);

                // Add Normal
                glm::vec3 n = calculateNormal(i, j, width, height, vertices);
                verticesWithNormals.push_back(n.x);
                verticesWithNormals.push_back(n.y);
                verticesWithNormals.push_back(n.z);
            }
        }
        vertices = verticesWithNormals; // Replace old vector
    }

    // Add this helper inside TerrainGeneration or above it
    glm::vec3 calculateNormal(int x, int z, int width, int height, const std::vector<float> &verts)
    {
        // Get heights of neighbors (with bounds checking)
        float hL = (x > 0) ? verts[((x - 1) * width + z) * 3 + 1] : verts[(x * width + z) * 3 + 1];
        float hR = (x < height - 1) ? verts[((x + 1) * width + z) * 3 + 1] : verts[(x * width + z) * 3 + 1];
        float hD = (z > 0) ? verts[(x * width + (z - 1)) * 3 + 1] : verts[(x * width + z) * 3 + 1];
        float hU = (z < width - 1) ? verts[(x * width + (z + 1)) * 3 + 1] : verts[(x * width + z) * 3 + 1];

        // Deduce normal from height difference
        // The constant (2.0) should match your grid spacing for perfect accuracy
        glm::vec3 normal = glm::normalize(glm::vec3(hL - hR, 2.0f, hD - hU));
        return normal;
    }

    float getHeight()
    {
        return this->height;
    }

    float getWidth()
    {
        return this->width;
    }

    float getHeight(float worldX, float worldZ)
    {
        float xPercent = (worldX / targetLength) + 0.5f;
        float zPercent = (worldZ / targetWidth) + 0.5f;

        int col = (int)(xPercent * (height - 1));
        int row = (int)(zPercent * (width - 1));

        if (col < 0)
            col = 0;
        if (col >= height)
            col = height - 1;
        if (row < 0)
            row = 0;
        if (row >= width)
            row = width - 1;

        int vertexIndex = (col * width + row) * 6;
        return vertices[vertexIndex + 1];
    }

    std::vector<float> &getVertices()
    {
        return vertices;
    };
    std::vector<unsigned> &getIndices()
    {
        return indices;
    };
};
#endif