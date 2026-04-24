
#include "propMaker.h"

#include "../renderer/objectRenderer.h"

std::shared_ptr<Prop> PropMaker::makeLamp(const glm::vec3& position, float radius, const glm::vec3& color,
    const glm::vec4& lightProperties, const glm::vec3& lightAttenuation)
{
    auto renderer = std::make_shared<ObjectRenderer>();
    auto sphere = Object::make(PATH_TO_SRC "/../assets/models/sphere_smooth.obj", renderer);
    auto light = Light::make();
    sphere->attach(light);
    sphere->setColor(color);
    light->setColor(color);
    light->setAttenuation(lightAttenuation.x, lightAttenuation.y, lightAttenuation.z);
    light->setProperties(lightProperties.x, lightProperties.y, lightProperties.z, lightProperties.w);
    sphere->setPosition(position);

    auto prop = std::make_shared<Prop>();

    prop->addEntity(sphere);
    prop->addEntity(light);
    prop->addRenderer(renderer);
    prop->setMainObject(sphere);

    return prop;
}
