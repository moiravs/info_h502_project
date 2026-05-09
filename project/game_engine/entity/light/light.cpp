#include "light.h"

#include "../../manager/lightManager.h"


Light::Light(const float x, const float y, const float z)
: Entity()
{
    this->Light::setPosition({x, y, z});
}

void Light::setPosition(const glm::vec3& position)
{
    this->Entity::setPosition(position);

    LightManager::get().notify();
}

void Light::setColor(const glm::vec3& color)
{
    this->_color = color;

    LightManager::get().notify();
}

glm::vec3 Light::getColor() const
{
    return this->_color;
}
