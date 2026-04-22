#include "renderer.h"

#include <utility>

#include "../../utils/utils.h"

Renderer::Renderer(std::shared_ptr<Shader> shader): _shader(std::move(shader))
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
}

void Renderer::registerObject(std::shared_ptr<Object> object)
{
    if (this->_object)
    {
        ERROR("An object is already assigned to this renderer.");
        return;
    }

    this->_object = std::move(object);
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
