#ifndef WaterRenderer_H
#define WaterRenderer_H

#include <vector>

#include "../entity/object.h"
#include "../shader.h"
#include "../waterFrameBuffer.h"
#include "renderer.h"
#include "../texture.h"

class WaterRenderer : public Renderer
{
    Texture dudvMap = Texture(PATH_TO_SRC "/../assets/textures/waterdudv.png");

    bool transparent;
    std::shared_ptr<WaterFrameBuffer> _fbos;

protected:
    [[nodiscard]] std::string getShaderName() const override;
public:
    explicit WaterRenderer(std::shared_ptr<WaterFrameBuffer> fbos);

    void registerObject(std::shared_ptr<Object> object) override;
    void render() override;
};
#endif
