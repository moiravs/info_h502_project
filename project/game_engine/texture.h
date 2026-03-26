#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Texture
{
    int numStrips;
    int numTrisPerStrip;
    std::vector<float> vertices;
    std::vector<unsigned> indices;
    unsigned int terrainVAO, terrainVBO, terrainIBO;
    float targetWidth;
    float targetLength;
    int width;
    int height;

public:
    GLuint ID;

    Texture(const char *texturePath, float targetWidth, float targetLength)
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

        numStrips = (height - 1) / rez;
        numTrisPerStrip = (width / rez) * 2 - 2;
        // first, configure the cube's VAO (and terrainVBO + terrainIBO)
        glGenVertexArrays(1, &terrainVAO);
        glBindVertexArray(terrainVAO);

        glGenBuffers(1, &terrainVBO);
        glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &terrainIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);
    }

    void draw()
    {
        glBindVertexArray(terrainVAO);

        for (unsigned strip = 0; strip < numStrips; strip++)
        {
            glDrawElements(GL_TRIANGLE_STRIP,                                           // primitive type
                           numTrisPerStrip + 2,                                         // number of indices to render
                           GL_UNSIGNED_INT,                                             // index data type
                           (void *)(sizeof(unsigned) * (numTrisPerStrip + 2) * strip)); // offset to starting index
        }
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

    ~Texture()
    {
        glDeleteVertexArrays(1, &terrainVAO);
        glDeleteBuffers(1, &terrainVBO);
        glDeleteBuffers(1, &terrainIBO);
    }
};
#endif