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
    std::shared_ptr<Object> _model;
    Texture *_tex;
    GLuint _instanceVBO;
    unsigned int _instanceCount;
    std::vector<GLuint> _vaos;

public:
    InstancedRenderer(std::shared_ptr<Object> model, Texture *texture, const std::vector<glm::mat4> &matrices);
    void updateUniforms() const override;
    ~InstancedRenderer() override;
    void render() override;
};
#endif
