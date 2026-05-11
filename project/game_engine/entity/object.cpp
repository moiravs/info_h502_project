#include "object.h"

#include <fstream>
#include <sstream>
#include <glm/gtc/matrix_transform.hpp>

#include "../manager/mainCamera.h"
#include "../renderer/objectRenderer.h"
#include "../renderer/renderer.h"
#include "renderableEntityMaker.h"

std::shared_ptr<Object> Object::make(const std::shared_ptr<Mesh> &mesh, const std::string &shader)
{
    return RenderableEntityMaker::makeRenderable<Object, ObjectRenderer>(shader, mesh);
}

Object::Object(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Renderer> &renderer) : RenderableEntity(renderer)
{
    m_mesh = mesh;
    height = computeHeight();
}

const std::array<glm::vec3, 8>& Object::getBounds() const
{
    return this->_bounds;
}

void Object::setPosition(const glm::vec3& position)
{
    RenderableEntity::setPosition(position);
    this->updateBounds();
}

void Object::setRotation(const float yaw, const float pitch, const float roll)
{
    RenderableEntity::setRotation(yaw, pitch, roll);
    this->updateBounds();
}

bool Object::shouldRender() const
{
    return MainCamera::get()->canView(this->getBounds());
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

    const auto model = this->getModel();

    for (int i = 0; i < 8; ++i) {
        this->_bounds.at(i) = glm::vec3(model * glm::vec4(corners[i], 1.0));
    }
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

void Object::setMaterial(const float specular, const float shininess)
{
    const float spec = glm::clamp(specular, 0.f, 1.f);
    const float shin = glm::clamp(shininess, 0.f, 256.f);
    this->_material = glm::vec2(spec, shin / 256);
}

glm::vec2 Object::getMaterial() const
{
    return this->_material;
}
