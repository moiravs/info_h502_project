#ifndef InstancedRenderer_H
#define InstancedRenderer_H

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../entity/object.h"
#include "../texture.h"
#include "../shader.h"
#include "renderer.h"

class InstancedRenderer : public Renderer
{
    GLuint _instanceVBO{};
    int _vertexCount;
    int _instanceCount;
    Texture *_tex;
public:

    InstancedRenderer(std::shared_ptr<Shader> shader, std::shared_ptr<Object> model, Texture *texture, const std::vector<glm::mat4> &matrices);

    ~InstancedRenderer() override;
    void render() override;
};
#endif
