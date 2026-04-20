#include "light.h"

Light::Light() : Light(0, 0, 0)
{

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
}

void Light::setAttenuation(const float constant, const float linear, const float quadratic)
{
    _constant = constant;
    _linear = linear;
    _quadratic = quadratic;
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
