
#include "renderableEntity.h"

#include "../renderer/renderer.h"
#include "../culling/octree.h"

RenderableEntity::RenderableEntity(const std::shared_ptr<Renderer> &renderer)
: Entity(), _renderer(renderer), _dirty(true)
{}

std::shared_ptr<Renderer> RenderableEntity::getRenderer() const
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
    if (const auto obj = std::dynamic_pointer_cast<RenderableEntity>(this->shared_from_this()))
    {
        this->octreeNode->moveObject(obj, position);
    }
    this->dirty();
}

void RenderableEntity::setRotation(const float yaw, const float pitch)
{
    Entity::setRotation(yaw, pitch);
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
    this->_renderer->render();
}

void RenderableEntity::setOctreeNode(const std::shared_ptr<Octree>& node)
{
    this->octreeNode = node;
}

void RenderableEntity::update(const float delta)
{
    this->_renderer->updateUniforms();
}

bool RenderableEntity::shouldUpdate() const
{
    return this->isDirty();
}
