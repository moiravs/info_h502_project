#include "terrainRenderer.h"

#include "../displaymanager.h"

TerrainRenderer::TerrainRenderer(TerrainGeneration& terrain_gen, const Shader& shader): m_texture(terrain_gen), _shader(shader)
{
    const std::vector<float>& vertices = terrain_gen.getVertices();
    const std::vector<unsigned>& indices = terrain_gen.getIndices();

    numStrips = (terrain_gen.getHeight() - 1);
    numTrisPerStrip = (terrain_gen.getWidth()) * 2 - 2;

    glGenVertexArrays(1, &terrainVAO);
    glBindVertexArray(terrainVAO);

    glGenBuffers(1, &terrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    // Inside TerrainRenderer constructor
    constexpr int stride = 6 * sizeof(float);

    // Position (Location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, nullptr);
    glEnableVertexAttribArray(0);

    // Normal (Location 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &terrainIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);
}

void TerrainRenderer::render()
{
    _shader.use();
    _shader.updatePos(camera);
    glBindVertexArray(terrainVAO);

    for (unsigned strip = 0; strip < numStrips; strip++)
    {
        glDrawElements(GL_TRIANGLE_STRIP,                                           // primitive type
                       numTrisPerStrip + 2,                                         // number of indices to render
                       GL_UNSIGNED_INT,                                             // index data type
                       reinterpret_cast<void*>(sizeof(unsigned) * (numTrisPerStrip + 2) * strip)); // offset to starting index
    }
}

TerrainRenderer::~TerrainRenderer()
{
    glDeleteVertexArrays(1, &terrainVAO);
    glDeleteBuffers(1, &terrainVBO);
    glDeleteBuffers(1, &terrainIBO);
}
