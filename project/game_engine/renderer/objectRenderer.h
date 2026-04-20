#ifndef ObjectRenderer_H
#define ObjectRenderer_H

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include "../entity/object.h"
#include "../texture.h"
#include "renderer.h"
#include "../shader.h"

class ObjectRenderer : public Renderer
{
    GLuint VBO{}, VAO{};
    bool _transparent{};
    std::vector<glm::mat4> _models;
    Shader _shader;
    Texture _tex;
    std::vector<Object *> _objects;
    glm::mat4 model = glm::mat4(1.0);
public:

    ObjectRenderer(const Shader& shader, Object *object);

    ObjectRenderer(const Shader& shader, Object *object, const Texture& tex);

    ObjectRenderer(const Shader& shader, const std::vector<Object *>& objects, const Texture& tex);

    void initObjectRenderer();

    void render() override;
};
#endif
