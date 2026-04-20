#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>

#include <glad/glad.h>

#include "terrainGeneration.h"
#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 Position;
    glm::vec2 Texture;
    glm::vec3 Normal;
};

class Object
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textures;
    std::vector<glm::vec3> normals;
    std::vector<Vertex> vertices;

    GLuint VBO, VAO;
    bool transparent;

    glm::mat4 model = glm::mat4(1.0);
    glm::vec3 worldPos{};
public:
    explicit Object(const char *path, bool transparent = false);

    Object(float size, float height, bool transparent = false);

    std::vector<glm::vec3> getPositions();

    std::vector<glm::vec2> getTextures();

    std::vector<glm::vec3> getNormals();

    std::vector<Vertex> getVertices();

    [[nodiscard]] const glm::mat4& getModel() const;

    [[nodiscard]] size_t getNumVertices() const;

    void setWorldPosition(float x, float z, const TerrainGeneration &terrain);

    void setWorldPosition(float x, float y, float z);

    [[nodiscard]] float getHeight() const;
};
#endif