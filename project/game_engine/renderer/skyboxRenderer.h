#ifndef SkyboxRenderer_H
#define SkyboxRenderer_H

#include <vector>

#include <glm/glm.hpp>
#include "../entity/skybox.h"
#include "../shader/shader.h"
#include "renderer.h"

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
