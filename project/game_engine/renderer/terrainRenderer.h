
#ifndef TerrainRenderer_H
#define TerrainRenderer_H

#include "renderer.h"
#include "../mesh/terrainMesh.h"

class TerrainRenderer : public Renderer
{
    GLuint terrainIBO{};
    std::shared_ptr<TerrainMesh> m_texture;
    int numStrips;
    int numTrisPerStrip;
public:
    explicit TerrainRenderer(const std::shared_ptr<TerrainMesh>& terrain);
    void updateUniforms() const override;
    void render() override;
    ~TerrainRenderer() override;
};

#endif