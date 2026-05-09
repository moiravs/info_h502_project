
#include "directionalLight.h"

#include <iostream>

#include "../../manager/lightManager.h"
#include "../../manager/mainCamera.h"
#include "glm/gtx/string_cast.hpp"

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

glm::mat4 DirectionalLight::getPV() const
{
    const auto viewCorners = MainCamera::get()->getFrustumCorners();
    glm::vec3 center(0.0f);
    for (const auto& v : viewCorners)
        center += v;

    center /= viewCorners.size();

    const auto view = glm::lookAt(center - this->getDirection(), center, WORLD_UP);
    float minX = FLT_MAX;
    float maxX = -FLT_MAX;
    float minY = FLT_MAX;
    float maxY = -FLT_MAX;
    float minZ = FLT_MAX;
    float maxZ = -FLT_MAX;

    for (const auto& corner : viewCorners)
    {
        glm::vec4 trf = view * glm::vec4(corner, 1);
        minX = std::min(minX, trf.x);
        maxX = std::max(maxX, trf.x);
        minY = std::min(minY, trf.y);
        maxY = std::max(maxY, trf.y);
        minZ = std::min(minZ, trf.z);
        maxZ = std::max(maxZ, trf.z);
    }

    const auto proj = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

    return proj * view;
}

glm::vec3 DirectionalLight::getDirection() const
{
    return glm::normalize(this->_target - this->getPosition());
}
