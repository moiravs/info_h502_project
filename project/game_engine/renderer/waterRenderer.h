#ifndef WaterRenderer_H
#define WaterRenderer_H

#include <vector>

#include "meshRenderer.h"
#include "../entity/object.h"
#include "renderer.h"
#include "../texture.h"

class WaterRenderer : public MeshRenderer
{
    float WAVE_SPEED = 0.03;
    Texture dudvMap = Texture(PATH_TO_SRC "/../assets/textures/waterdudv.png");
    float moveFactor = 0;

    bool transparent;

public:
    WaterRenderer();
    void updateUniforms() const override;
    void registerEntity(const std::shared_ptr<RenderableEntity> &entity) override;
    void render() override;
};
#endif