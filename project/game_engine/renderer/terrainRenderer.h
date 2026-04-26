
#ifndef TerrainRenderer_H
#define TerrainRenderer_H

#include "../terrainGeneration.h"
#include "renderer.h"

class TerrainRenderer : public Renderer, public UboProvider
{

    struct Mist
    {
        glm::vec3 mistColor;
        float mistDensity;
        glm::vec3 cameraPos;
        float fogMaxHeight;
        float fogMinHeight;
        float fogDensity;
    };
    GLuint terrainIBO{};
    TerrainGeneration &m_texture;
    int numStrips;
    int numTrisPerStrip;

    Texture grassTex = Texture(PATH_TO_SRC "/../assets/textures/terrain/grass.jpg");
    Texture rockTex = Texture(PATH_TO_SRC "/../assets/textures/terrain/rock.jpg");
    Texture snowTex = Texture(PATH_TO_SRC "/../assets/textures/terrain/snow.jpg");

public:
    explicit TerrainRenderer(TerrainGeneration &terrain_gen);
    void updateUniforms() const override;
    void render() override;
    void updateUBO();
    ~TerrainRenderer() override;
};

#endif