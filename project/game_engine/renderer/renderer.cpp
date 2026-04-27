#include "renderer.h"

#include <utility>

#include "../../utils/utils.h"

Renderer::Renderer(std::shared_ptr<Shader> shader) : _shader(std::move(shader)) {}

Renderer::~Renderer()
{
    this->clearVAOs();
    this->clearVBOs();
}

void Renderer::registerEntity(const std::shared_ptr<RenderableEntity> &entity)
{
    if (this->_entity)
    {
        ERROR("An entity is already assigned to this renderer.");
        return;
    }

    this->_entity = entity;
    this->setupVAOs();
}

std::shared_ptr<Shader> Renderer::generateShader(const std::string &shaderName)
{
    const std::string fullName = PATH_TO_SRC "/../assets/shaders/" + shaderName;
    return std::make_shared<Shader>(fullName + ".vert", fullName + ".frag");
}

void Renderer::createVAOs(const size_t nb)
{
    if (!this->_VAOs.empty())
    {
        this->clearVAOs();
        this->_VAOs.clear();
    }

    this->_VAOs.resize(nb);
    glGenVertexArrays(nb, this->_VAOs.data());
}

void Renderer::clearVAOs() const
{
    if (!this->_VAOs.empty())
    {
        glDeleteVertexArrays(this->_VAOs.size(), this->_VAOs.data());
    }
}

void Renderer::createVBOs(const size_t nb)
{
    if (!this->_VBOs.empty())
    {
        this->clearVBOs();
        this->_VBOs.clear();
    }

    this->_VBOs.resize(nb);
    glGenBuffers(nb, this->_VBOs.data());
}

void Renderer::clearVBOs() const
{
    if (!this->_VBOs.empty())
        glDeleteBuffers(this->_VBOs.size(), this->_VBOs.data());
}

void Renderer::render()
{
    this->_shader->use();
    this->updateUniforms();
    this->_shader->use();
}
