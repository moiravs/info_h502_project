#include "terrainRenderer.h"

#include "../mainCamera.h"

TerrainRenderer::TerrainRenderer(TerrainGeneration& terrain_gen)
: Renderer(this->generateShader()), m_texture(terrain_gen)
{
    const std::vector<float>& vertices = terrain_gen.getVertices();
    const std::vector<unsigned>& indices = terrain_gen.getIndices();

    numStrips = (terrain_gen.getHeight() - 1);
    numTrisPerStrip = (terrain_gen.getWidth()) * 2 - 2;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
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
    _shader->updatePos(MainCamera::get());
    glBindVertexArray(VAO);

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
    Renderer::~Renderer();
    glDeleteBuffers(1, &terrainIBO);
}

std::string TerrainRenderer::getShaderName() const
{
    return "cpu_height";
}
