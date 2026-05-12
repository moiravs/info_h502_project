
#include "prop.h"

#include "../../utils/utils.h"

void Prop::render()
{
    if (!this->getMainObject())
    {
        for (const auto &r : this->_renderables)
            r->render();
    }
    else
        this->getMainObject()->render();
}

std::vector<std::shared_ptr<Renderable>> Prop::getRenderables()
{
    return _renderables;
}

void Prop::renderWithShader(const std::shared_ptr<Shader> &shader)
{
    for (const auto &r : this->_renderables)
        r->renderWithShader(shader);
}

void Prop::addRenderable(const std::shared_ptr<Renderable> &renderable)
{
    this->_renderables.push_back(renderable);
}

void Prop::setPosition(const glm::vec3 &position) const
{
    if (!this->_mainObject)
    {
        ERROR("No main object set for the prop.");
        return;
    }

    this->_mainObject->setPosition(position);
}

void Prop::update(const float delta)
{
    for (const auto &r : this->_renderables)
        r->update(delta);
}

void Prop::setMainObject(const std::shared_ptr<Object> &mainObject)
{
    this->_mainObject = mainObject;
}

std::shared_ptr<Object> Prop::getMainObject()
{
    return this->_mainObject;
}
