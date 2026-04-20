
#ifndef TerrainRenderer_H
#define TerrainRenderer_H

#include "../terrainGeneration.h"
#include "renderer.h"
#include "../shader.h"

class TerrainRenderer : public Renderer
{
    unsigned int terrainVAO{}, terrainVBO{}, terrainIBO{};
    TerrainGeneration &m_texture;
    int numStrips;
    int numTrisPerStrip;
    Shader _shader;
public:

    TerrainRenderer(TerrainGeneration &terrain_gen, const Shader& shader);

    void render() override;
    ~TerrainRenderer() override;
};

#endif