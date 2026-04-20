#ifndef InstancedRenderer_H
#define InstancedRenderer_H

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../object.h"
#include "../texture.h"
#include "../shader.h"
#include "renderer.h"

class InstancedRenderer : public Renderer
{
    GLuint VAO{}, VBO{}, _instanceVBO{};
    int _vertexCount;
    int _instanceCount;
    Shader _shader;
    Texture *_tex;
public:

    InstancedRenderer(const Shader& shader, Object &model, Texture *texture, const std::vector<glm::mat4> &matrices);

    void render() override;
};
#endif
