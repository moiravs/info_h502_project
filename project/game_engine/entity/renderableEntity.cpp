
#include "renderableEntity.h"

RenderableEntity::RenderableEntity(const std::shared_ptr<Renderer> &renderer)
: Entity(), _renderer(renderer), _dirty(true)
{
}

std::shared_ptr<Renderer> RenderableEntity::getRenderer()
{
    return this->_renderer;
}

glm::mat4 RenderableEntity::getModel() const
{
    auto model = glm::mat4(1.0f);

    model = glm::translate(model, this->getPosition());

    model = glm::rotate(model, this->getYaw(),   glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis
    model = glm::rotate(model, this->getPitch(), glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis

    return model;
}

void RenderableEntity::dirty()
{
    this->_dirty = true;
}

void RenderableEntity::setPosition(const glm::vec3& position)
{
    Entity::setPosition(position);
    this->dirty();
}

void RenderableEntity::setRotation(const float yaw, const float pitch)
{
    Entity::setRotation(yaw, pitch);
    this->dirty();
}
