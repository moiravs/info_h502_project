
#include "renderableEntity.h"

#include "../renderer/renderer.h"

RenderableEntity::RenderableEntity(const std::shared_ptr<Renderer> &renderer)
    : Entity(), _renderer(renderer), _dirty(true)
{
}

std::shared_ptr<Renderer> RenderableEntity::getRenderer() const
{
    return this->_renderer;
}

glm::mat4 RenderableEntity::getModel() const
{
    auto model = glm::mat4(1.0f);

    model = glm::translate(model, this->getPosition());

    model = glm::rotate(model, -this->getYaw(), glm::vec3(0.0f, 1.0f, 0.0f));   // Y-axis
    model = glm::rotate(model, -this->getPitch(), glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis
    model = glm::rotate(model, this->getRoll(), glm::vec3(0.0f, 0.0f, 1.0f));  // X-axis

    return model;
}

void RenderableEntity::dirty()
{
    this->_dirty = true;
}

void RenderableEntity::setPosition(const glm::vec3 &position)
{
    Entity::setPosition(position);
    this->dirty();
}

void RenderableEntity::setRotation(const float yaw, const float pitch, const float roll)
{
    Entity::setRotation(yaw, pitch, roll);
    this->dirty();
}

bool RenderableEntity::isDirty() const
{
    return this->_dirty;
}

void RenderableEntity::render(const float delta)
{
    if (this->shouldUpdate())
        this->update(delta);

    this->_dirty = false;

    if (this->shouldRender())
    {
        this->_renderer->render();
    }
}

bool RenderableEntity::shouldRender() const { return true; }

void RenderableEntity::update(const float delta)
{
    this->_renderer->updateUniforms();
}

bool RenderableEntity::shouldUpdate() const
{
    return this->isDirty();
}
