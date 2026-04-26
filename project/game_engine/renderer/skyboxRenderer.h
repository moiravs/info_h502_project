#ifndef SkyboxRenderer_H
#define SkyboxRenderer_H

#include <vector>

#include <glm/glm.hpp>
#include "renderer.h"
#include "../shader.h"
#include "../skybox.h"

class SkyboxRenderer : public Renderer
{
protected:
    void setupVAOs() override;
public:
    explicit SkyboxRenderer(const std::string& shader);
    void updateUniforms() const override;
    void render() override;
};
#endif
