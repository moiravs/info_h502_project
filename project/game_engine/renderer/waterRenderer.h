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
    std::vector<Vertex> vertices;

    Texture dudvMap = Texture(PATH_TO_SRC "/../assets/textures/waterdudv.png");

    size_t numVertices;

    GLuint VBO{}, VAO{};
    bool transparent;
    Shader _shader;
    Object _object;
    WaterFrameBuffer _fbos;

public:

    WaterRenderer(const Shader& shader, Object object, const WaterFrameBuffer& fbos);

    void draw() const;

    void render() override;
};
#endif
