#ifndef WaterRenderer_H
#define WaterRenderer_H

#include <vector>

#include <glad/glad.h>
#include "../entity/object.h"
#include "../shader.h"
#include "../waterFrameBuffer.h"
#include "renderer.h"
#include "../texture.h"

class WaterRenderer : public Renderer
{
    Texture dudvMap = Texture(PATH_TO_SRC "/../assets/textures/waterdudv.png");

    GLuint VBO{}, VAO{};
    bool transparent;
    Shader _shader;
    WaterFrameBuffer _fbos;

public:

    WaterRenderer(const Shader& shader, const WaterFrameBuffer& fbos);

    void registerObject(std::shared_ptr<Object> object) override;
    void draw() const;
    void render() override;
};
#endif
