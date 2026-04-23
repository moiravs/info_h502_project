#include "light.h"

#include "../manager/lightManager.h"

std::shared_ptr<Light> Light::make()
{
    return Light::make(0, 0, 0);
}

std::shared_ptr<Light> Light::make(const float x, const float y, const float z)
{
    const auto ret = std::make_shared<Light>(Light(x, y, z));
    LightManager::get()->registerLight(ret);
    return ret;
}

Light::Light(const float x, const float y, const float z) : Entity(), _ambient(0), _diffuse(0), _specular(0), _constant(0),
                                                            _linear(0), _quadratic(0)
{
    this->Entity::setPosition(x, y, z);
}

void Light::setProperties(const float ambient, const float diffuse, const float specular)
{
    _ambient = ambient;
    _diffuse = diffuse;
    _specular = specular;

    LightManager::get()->notify();
}

void Light::setAttenuation(const float constant, const float linear, const float quadratic)
{
    _constant = constant;
    _linear = linear;
    _quadratic = quadratic;

    LightManager::get()->notify();
}

float Light::getAmbient() const
{
    return _ambient;
}

float Light::getSpecular() const
{
    return _specular;
}

float Light::getDiffuse() const
{
    return _diffuse;
}

float Light::getConstant() const
{
    return _constant;
}

float Light::getLinear() const
{
    return _linear;
}

float Light::getQuadratic() const
{
    return _quadratic;
}
