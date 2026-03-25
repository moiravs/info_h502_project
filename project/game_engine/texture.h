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

public:
    GLuint ID;

    Texture(const char *texturePath)
    {
        stbi_set_flip_vertically_on_load(true);
        int width, height, nrChannels;
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

                // vertex
                vertices.push_back(-height / 2.0f + height * i / (float)height); // vx
                vertices.push_back((int)y * yScale - yShift);                    // vy
                vertices.push_back(-width / 2.0f + width * j / (float)width);    // vz
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

    ~Texture()
    {
        glDeleteVertexArrays(1, &terrainVAO);
        glDeleteBuffers(1, &terrainVBO);
        glDeleteBuffers(1, &terrainIBO);
    }
};
#endif