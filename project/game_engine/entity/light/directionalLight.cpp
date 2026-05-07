
#include "directionalLight.h"

#include "../../manager/lightManager.h"

DirectionalLight::DirectionalLight(float x, float y, float z)
: Light(x, y, z), _target(0) {  }

std::shared_ptr<DirectionalLight> DirectionalLight::make(const float x, const float y, const float z)
{
    auto ret = std::make_shared<DirectionalLight>(DirectionalLight(x, y, z));
    LightManager::get().registerDirectionalLight(ret);
    return ret;
}

glm::vec3 DirectionalLight::getTarget() const
{
    return this->_target;
}

void DirectionalLight::setTarget(const glm::vec3& target)
{
    this->_target = target;
}
