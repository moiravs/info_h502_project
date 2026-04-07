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

    TerrainGeneration(Texture texture, float targetWidth, float targetLength)
    {

        this->targetWidth = targetWidth;
        this->targetLength = targetLength;

        float yScale = 64.0f / 256.0f, yShift = 16.0f;
        int rez = 1;
        unsigned bytePerPixel = texture.getChannels();
        unsigned char *data = texture.getData();

        width = texture.getWidth();
        height = texture.getHeight();

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

        int vertexIndex = (col * width + row) * 3;
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