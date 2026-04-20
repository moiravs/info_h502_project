#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "entity.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec2 Texture;
    glm::vec3 Normal;
};

class Object : public Entity
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textures;
    std::vector<glm::vec3> normals;
    std::vector<Vertex> vertices;
    glm::mat4 model = glm::mat4(1.0);

    GLuint VBO, VAO;
    bool transparent;
public:
    explicit Object(const char *path, bool transparent = false);

    Object(float size, float height, bool transparent = false);

    std::vector<glm::vec3> getPositions();

    std::vector<glm::vec2> getTextures();

    std::vector<glm::vec3> getNormals();

    std::vector<Vertex> getVertices();

    // this weird line is there so that the overloads of Entity::setPosition stay despite the override
    using Entity::setPosition;
    void setPosition(const glm::vec3& position) override;

    [[nodiscard]] size_t getNumVertices() const;

    [[nodiscard]] float getHeight() const;

    [[nodiscard]] const glm::mat4& getModel() const;
};
#endif