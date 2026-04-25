#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "entity.h"
#include "../renderer/renderer.h"
#include "../culling/octree.h"
#include <memory>

#include "renderableEntity.h"
#include "../modelLoader.h"
#include "vertex.h"

class Octree;
class Renderer;

class Object : public RenderableEntity
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> textures;
    std::vector<glm::vec3> normals;
    std::vector<Vertex> vertices;

    glm::vec3 _color;

    float height = 0;
    glm::vec3 _scale;
    float computeHeight() const;

    std::shared_ptr<Mesh> m_mesh = nullptr;
    std::shared_ptr<Octree> octreeNode = nullptr;

public:
    explicit Object(const char *path, const std::shared_ptr<Renderer>& renderer);
    Object(float size, float height, const std::shared_ptr<Renderer>& renderer);

    static std::shared_ptr<Object> make(const char *path, const std::string &shader);
    static std::shared_ptr<Object> make(float size, float height, const std::string &shader);

    // this weird line is there so that the overloads of Entity::setPosition stay despite the override
    using Entity::setPosition;
    void setPosition(const glm::vec3 &position) override;

    void setColor(const glm::vec3 &color);

    void setScale(const glm::vec3 &scale);

    void setOctreeNode(const std::shared_ptr<Octree>& node);

    void render() override;

    [[nodiscard]] float getHeight() const;

    [[nodiscard]] glm::vec3 getColor() const;

    [[nodiscard]] glm::mat4 getModel() const override;

    std::shared_ptr<Mesh> getMesh() const;
};
#endif