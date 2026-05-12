
#ifndef INFOH502_CPP_POINTLIGHT_H
#define INFOH502_CPP_POINTLIGHT_H

#include "light.h"

class PointLight: public Light {
    float _ambient, _diffuse, _specular;
    float _constant, _linear, _quadratic;
    PointLight(float x, float y, float z);

public:
    static std::shared_ptr<PointLight> make(float x = 0, float y = 0, float z = 0);
    void setProperties(float ambient, float diffuse, float specular);

    void setAttenuation(float constant, float linear, float quadratic);

    [[nodiscard]] float getAmbient() const;

    [[nodiscard]] float getSpecular() const;

    [[nodiscard]] float getDiffuse() const;

    [[nodiscard]] float getConstant() const;

    [[nodiscard]] float getLinear() const;

    [[nodiscard]] float getQuadratic() const;
};



#endif //INFOH502_CPP_POINTLIGHT_H
