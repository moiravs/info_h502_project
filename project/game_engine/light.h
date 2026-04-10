#ifndef Light_H
#define Light_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Light
{
    glm::vec3 pos;
    float _ambient, _diffuse, _specular;
    float _constant, _linear, _quadratic;

public:
    Light(float x, float y, float z)
    {
        pos = glm::vec3(x, y, z);
    }

    void setProperties(float ambient, float diffuse, float specular)
    {
        _ambient = ambient;
        _diffuse = diffuse;
        _specular = specular;
    }

    void setAttenuation(float constant, float linear, float quadratic)
    {
        _constant = constant;
        _linear = linear;
        _quadratic = quadratic;
    }

    float getAmbient()
    {
        return _ambient;
    }

    float getSpecular()
    {
        return _specular;
    }
    float getDiffuse()
    {
        return _diffuse;
    }

    glm::vec3 getPos()
    {
        return pos;
    }

    void setPos(glm::vec3 m_pos)
    {
        pos = m_pos;
    }
};

#endif