
#include "prop.h"

#include "../../utils/utils.h"

void Prop::render(const float delta)
{
    for (const auto& r: this->_renderables)
        r->render(delta);
}

void Prop::addEntity(const std::shared_ptr<Entity>& entity)
{
    this->_entities.push_back(entity);
}

void Prop::addRenderable(const std::shared_ptr<Renderable>& renderer)
{
    this->_renderables.push_back(renderer);
}

void Prop::setPosition(const glm::vec3& position) const
{
    if (!this->_mainObject)
    {
        ERROR("No main object set for the prop.");
        return;
    }

    this->_mainObject->setPosition(position);
}

void Prop::setMainObject(const std::shared_ptr<Object>& mainObject)
{
    this->_mainObject = mainObject;
}

std::shared_ptr<Object> Prop::getMainObject()
{
    return this->_mainObject;
}
