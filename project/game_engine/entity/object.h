#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <glm/glm.hpp>

#include "entity.h"
#include "../culling/octree.h"
#include <memory>

#include "renderableEntity.h"
#include "../mesh/mesh.h"
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
public:
    explicit Object(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Renderer>& renderer);

    static std::shared_ptr<Object> make(const std::shared_ptr<Mesh> &mesh, const std::string &shader);

    // this weird line is there so that the overloads of Entity::setPosition stay despite the override
    using Entity::setPosition;
    void setPosition(const glm::vec3 &position) override;

    void setColor(const glm::vec3 &color);

    void setScale(const glm::vec3 &scale);

    [[nodiscard]] float getHeight() const;

    [[nodiscard]] glm::vec3 getColor() const;

    [[nodiscard]] glm::mat4 getModel() const override;

    std::shared_ptr<Mesh> getMesh() const;
};
#endif