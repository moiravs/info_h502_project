#ifndef WaterRenderer_H
#define WaterRenderer_H

#include <vector>

#include "meshRenderer.h"
#include "../entity/object.h"
#include "../waterFrameBuffer.h"
#include "renderer.h"
#include "../texture.h"

class WaterRenderer : public MeshRenderer
{
    Texture dudvMap = Texture(PATH_TO_SRC "/../assets/textures/waterdudv.png");

    bool transparent;
    std::shared_ptr<WaterFrameBuffer> _fbos;

public:
    explicit WaterRenderer(std::shared_ptr<WaterFrameBuffer> fbos);
    void updateUniforms() const override;
    void registerEntity(const std::shared_ptr<RenderableEntity> &entity) override;
    void render() override;
};
#endif