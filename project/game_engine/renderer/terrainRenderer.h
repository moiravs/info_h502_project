
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

    TerrainRenderer(std::shared_ptr<Shader> shader, TerrainGeneration &terrain_gen);

    void render() override;
    ~TerrainRenderer() override;
};

#endif