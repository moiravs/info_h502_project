#include "terrainRenderer.h"

TerrainRenderer::TerrainRenderer(const std::shared_ptr<TerrainMesh>& terrain)
    : Renderer(this->generateShader("cpu_height")), m_texture(terrain), numStrips(0), numTrisPerStrip(0)
{
}

void TerrainRenderer::updateUniforms() const
{}

void TerrainRenderer::render()
{
    Renderer::render();

    glBindVertexArray(_VAOs[0]);

    for (unsigned strip = 0; strip < numStrips; strip++)
    {
        glDrawElements(GL_TRIANGLE_STRIP,                                                           // primitive type
                       numTrisPerStrip + 2,                                                         // number of indices to render
                       GL_UNSIGNED_INT,                                                             // index data type
                       reinterpret_cast<void *>(sizeof(unsigned) * (numTrisPerStrip + 2) * strip)); // offset to starting index
    }
}

TerrainRenderer::~TerrainRenderer()
{
    glDeleteBuffers(1, &terrainIBO);
}
