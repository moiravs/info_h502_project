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
    glm::vec3 _color{};

    float height = 0;
    glm::vec3 _scale = glm::vec3(1);
    float computeHeight() const;
    glm::vec3 _minBound{};
    glm::vec3 _maxBound{};

    std::shared_ptr<Mesh> m_mesh = nullptr;

protected:
    void updateBounds();
public:
    explicit Object(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Renderer> &renderer);

    static std::shared_ptr<Object> make(const char *path, const std::shared_ptr<Renderer> &renderer = nullptr);
    static std::shared_ptr<Object> make(float size, float height, const std::shared_ptr<Renderer> &renderer = nullptr);
    static std::shared_ptr<Object> make(const std::shared_ptr<Mesh> &mesh, const std::string &shader);

    // this weird line is there so that the overloads of Entity::setPosition stay despite the override
    using Entity::setPosition;
    void setPosition(const glm::vec3 &position) override;

    void setColor(const glm::vec3 &color);

    void setScale(const glm::vec3 &scale);

    std::pair<glm::vec3, glm::vec3> getBounds() const;

    [[nodiscard]] float getHeight() const;

    [[nodiscard]] glm::vec3 getColor() const;

    [[nodiscard]] glm::mat4 getModel() const override;

    void update(float delta) override;

    std::shared_ptr<Mesh> getMesh() const;
};
#endif