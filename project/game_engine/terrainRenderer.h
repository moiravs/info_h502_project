
#ifndef TerrainRenderer_H
#define TerrainRenderer_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "terrainGeneration.h"
#include <vector>
class TerrainRenderer
{
public:
    unsigned int terrainVAO, terrainVBO, terrainIBO;
    TerrainGeneration &m_texture;
    int numStrips;
    int numTrisPerStrip;

    TerrainRenderer(TerrainGeneration &terrain_gen) : m_texture(terrain_gen)
    {
        std::vector<float> vertices = terrain_gen.getVertices();
        std::vector<unsigned> indices = terrain_gen.getIndices();

        numStrips = (terrain_gen.getHeight() - 1);
        numTrisPerStrip = (terrain_gen.getWidth()) * 2 - 2;

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

    ~TerrainRenderer()
    {
        glDeleteVertexArrays(1, &terrainVAO);
        glDeleteBuffers(1, &terrainVBO);
        glDeleteBuffers(1, &terrainIBO);
    }
};

#endif