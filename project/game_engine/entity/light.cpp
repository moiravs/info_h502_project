#include "light.h"

#include "../manager/lightManager.h"

std::shared_ptr<Light> Light::make()
{
    return Light::make(0, 0, 0);
}

std::shared_ptr<Light> Light::make(const float x, const float y, const float z)
{
    const auto ret = std::make_shared<Light>(Light(x, y, z));
    LightManager::get().registerLight(ret);
    return ret;
}

Light::Light(const float x, const float y, const float z)
: Entity(), _ambient(0), _diffuse(0), _specular(0), _shininess(1),
_constant(0), _linear(0), _quadratic(0)
{
    this->Light::setPosition(glm::vec3(x, y, z));
}

void Light::setPosition(const glm::vec3& position)
{
    this->Entity::setPosition(position);

    LightManager::get().notify();
}

void Light::setProperties(const float ambient, const float diffuse, const float specular, const float shininess)
{
    this->_ambient = ambient;
    this->_diffuse = diffuse;
    this->_specular = specular;
    this->_shininess = shininess;

    LightManager::get().notify();
}

void Light::setAttenuation(const float constant, const float linear, const float quadratic)
{
    this->_constant = constant;
    this->_linear = linear;
    this->_quadratic = quadratic;

    LightManager::get().notify();
}

float Light::getAmbient() const
{
    return this->_ambient;
}

float Light::getSpecular() const
{
    return this->_specular;
}

float Light::getDiffuse() const
{
    return this->_diffuse;
}

float Light::getConstant() const
{
    return this->_constant;
}

float Light::getLinear() const
{
    return this->_linear;
}

float Light::getQuadratic() const
{
    return this->_quadratic;
}

float Light::getShininess() const
{
    return this->_shininess;
}
