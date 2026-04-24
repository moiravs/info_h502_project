
#ifndef TerrainRenderer_H
#define TerrainRenderer_H

#include "../terrainGeneration.h"
#include "renderer.h"
#include "../shader.h"

class TerrainRenderer : public Renderer
{
    GLuint terrainIBO{};
    TerrainGeneration &m_texture;
    int numStrips;
    int numTrisPerStrip;

public:
    explicit TerrainRenderer(TerrainGeneration &terrain_gen);
    void updateUniforms() const override;
    void render() override;
    ~TerrainRenderer() override;
};

#endif