#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 Position;
    glm::vec2 Texture;
    glm::vec3 Normal;
};

#endif