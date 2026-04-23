#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "entity.h"
#include "../renderer/renderer.h"
#include <memory>

struct Vertex
{
    glm::vec3 Position;
    glm::vec2 Texture;
    glm::vec3 Normal;
};

class Renderer;

class Object : public Entity
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textures;
    std::vector<glm::vec3> normals;
    std::vector<Vertex> vertices;
    glm::mat4 model = glm::mat4(1.0);

    float height = 0;
    float computeHeight() const;

public:
    explicit Object(const char *path);
    Object(float size, float height);

    static std::shared_ptr<Object> make(const char *path, std::shared_ptr<Renderer> renderer = nullptr);
    static std::shared_ptr<Object> make(float size, float height, std::shared_ptr<Renderer> renderer = nullptr);

    std::vector<glm::vec3> getPositions();

    std::vector<glm::vec2> getTextures();

    std::vector<glm::vec3> getNormals();

    std::vector<Vertex> getVertices();

    // this weird line is there so that the overloads of Entity::setPosition stay despite the override
    using Entity::setPosition;
    void setPosition(const glm::vec3 &position) override;

    [[nodiscard]] size_t getNumVertices() const;

    [[nodiscard]] float getHeight() const;

    [[nodiscard]] const glm::mat4 &getModel() const;

    void registerRenderer(const std::shared_ptr<Renderer> &renderer);
};
#endif