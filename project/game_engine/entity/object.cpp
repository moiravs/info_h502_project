#include "object.h"

#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

#include "renderableEntityMaker.h"
#include "../renderer/objectRenderer.h"
#include "../renderer/renderer.h"

std::shared_ptr<Object> Object::make(const std::shared_ptr<Mesh> &mesh, const std::string &shader)
{
    return RenderableEntityMaker::makeRenderable<Object, ObjectRenderer>(shader, mesh);
}

Object::Object(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Renderer> &renderer) : RenderableEntity(renderer)
{
    m_mesh = mesh;
    height = computeHeight();
}

void Object::updateBounds()
{
    if (!this->m_mesh) return;

    auto [min, max] = this->m_mesh->getBounds();
    const glm::vec3 corners[8] = {
        {min.x, min.y, min.z},
        {max.x, min.y, min.z},
        {max.x, max.y, min.z},
        {min.x, max.y, min.z},
        {min.x, min.y, max.z},
        {max.x, min.y, max.z},
        {max.x, max.y, max.z},
        {min.x, max.y, max.z}
    };

    glm::vec3 worldCorners[8];
    const auto model = this->getModel();
    this->_minBound = glm::vec3(model * glm::vec4(corners[0], 1.0));
    this->_maxBound = glm::vec3(model * glm::vec4(corners[0], 1.0));

    for (int i = 1; i < 8; ++i) {
        auto mapped = glm::vec3(model * glm::vec4(corners[i], 1.0));
        this->_minBound = glm::min(this->_minBound, mapped);
        this->_maxBound = glm::max(this->_maxBound, mapped);
    }
}

std::pair<glm::vec3, glm::vec3> Object::getBounds() const
{
    return {this->_minBound, this->_maxBound};
}

float Object::computeHeight() const
{
    if (!this->m_mesh) return 0;

    auto [min, max] = this->m_mesh->getBounds();
    return max.y - min.y;
}

float Object::getHeight() const
{
    return height;
}

void Object::setPosition(const glm::vec3 &position)
{
    this->RenderableEntity::setPosition(position);
}

void Object::setScale(const glm::vec3 &scale)
{
    this->_scale = scale;
    this->dirty();
}

void Object::update(const float delta)
{
    this->updateBounds();
    RenderableEntity::update(delta);
}

glm::mat4 Object::getModel() const
{
    return glm::scale(RenderableEntity::getModel(), this->_scale);
}

glm::vec3 Object::getColor() const
{
    return this->_color;
}

void Object::setColor(const glm::vec3 &color)
{
    this->_color = color;
    this->dirty();
}

std::shared_ptr<Mesh> Object::getMesh() const
{
    return this->m_mesh;
}