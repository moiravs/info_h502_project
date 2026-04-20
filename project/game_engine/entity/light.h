#ifndef Light_H
#define Light_H

#include "entity.h"

class Light : public Entity
{
    float _ambient, _diffuse, _specular;
    float _constant, _linear, _quadratic;

public:
    Light(float x, float y, float z);
    Light();

    void setProperties(float ambient, float diffuse, float specular);

    void setAttenuation(float constant, float linear, float quadratic);

    [[nodiscard]] float getAmbient() const;

    [[nodiscard]] float getSpecular() const;

    [[nodiscard]] float getDiffuse() const;
};

#endif