#ifndef SkyboxRenderer_H
#define SkyboxRenderer_H

#include <vector>

#include <glm/glm.hpp>
#include "renderer.h"
#include "../shader.h"
#include "../skybox.h"

class SkyboxRenderer : public Renderer
{
    bool _transparent;
    std::vector<glm::mat4> _models;
    Skybox *_skybox;
    glm::mat4 model = glm::mat4(1.0);
    unsigned int cubemapTexture;
protected:
    [[nodiscard]] std::string getShaderName() const override;

public:
    explicit SkyboxRenderer(Skybox *skybox);
    void updateUniforms() const override;
    void render() override;
};
#endif
