#ifndef Light_H
#define Light_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light
{
    glm::vec3 pos;
    float _ambient, _diffuse, _specular;
    float _constant, _linear, _quadratic;

public:
    Light(float x, float y, float z);

    void setProperties(float ambient, float diffuse, float specular);

    void setAttenuation(float constant, float linear, float quadratic);

    [[nodiscard]] float getAmbient() const;

    [[nodiscard]] float getSpecular() const;

    [[nodiscard]] float getDiffuse() const;

    [[nodiscard]] glm::vec3 getPos() const;

    void setPos(glm::vec3 m_pos);
};

#endif