
#include "pointLight.h"

#include "../../manager/lightManager.h"

std::shared_ptr<PointLight> PointLight::make(const float x, const float y, const float z)
{
    const auto ret = std::make_shared<PointLight>(PointLight(x, y, z));
    LightManager::get().registerPointLight(ret);
    return ret;
}

PointLight::PointLight(const float x, const float y, const float z)
: Light(x, y, z), _ambient(0), _diffuse(0), _specular(0), _shininess(1),
_constant(0), _linear(0), _quadratic(0)
{}

void PointLight::setProperties(const float ambient, const float diffuse, const float specular, const float shininess)
{
    this->_ambient = ambient;
    this->_diffuse = diffuse;
    this->_specular = specular;
    this->_shininess = shininess;

    LightManager::get().notify();
}

void PointLight::setAttenuation(const float constant, const float linear, const float quadratic)
{
    this->_constant = constant;
    this->_linear = linear;
    this->_quadratic = quadratic;

    LightManager::get().notify();
}

float PointLight::getAmbient() const
{
    return this->_ambient;
}

float PointLight::getSpecular() const
{
    return this->_specular;
}

float PointLight::getDiffuse() const
{
    return this->_diffuse;
}

float PointLight::getConstant() const
{
    return this->_constant;
}

float PointLight::getLinear() const
{
    return this->_linear;
}

float PointLight::getQuadratic() const
{
    return this->_quadratic;
}

float PointLight::getShininess() const
{
    return this->_shininess;
}