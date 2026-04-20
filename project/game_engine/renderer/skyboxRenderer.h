#ifndef SkyboxRenderer_H
#define SkyboxRenderer_H

#include <vector>

#include <glm/glm.hpp>
#include "renderer.h"
#include "../shader.h"
#include "../skybox.h"

class SkyboxRenderer : public Renderer
{
    unsigned int skyboxVAO{}, skyboxVBO{};
    bool _transparent;
    std::vector<glm::mat4> _models;
    Shader _shader;
    Skybox *_skybox;
    glm::mat4 model = glm::mat4(1.0);
    unsigned int cubemapTexture;

public:
    SkyboxRenderer(const Shader& shader, Skybox *skybox);

    void render() override;
};
#endif
