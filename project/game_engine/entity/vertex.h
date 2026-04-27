#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texture;
    glm::vec3 normal;
};

#endif