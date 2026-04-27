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

Object::Object(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Renderer>& renderer) : RenderableEntity(renderer)
{
    m_mesh = mesh;
    height = computeHeight();
}

float Object::computeHeight() const
{
    if (vertices.empty())
        return 0.0f;

    float minY = vertices[0].position.y;
    float maxY = vertices[0].position.y;

    for (const auto &vertex : vertices)
    {
        if (vertex.position.y < minY)
            minY = vertex.position.y;
        if (vertex.position.y > maxY)
            maxY = vertex.position.y;
    }

    return maxY - minY;
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
