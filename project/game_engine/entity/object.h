#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <glm/glm.hpp>

#include "entity.h"
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

    std::shared_ptr<Mesh> m_mesh = nullptr;

protected:
    std::array<glm::vec3, 8> _bounds{};
public:
    void updateBounds();
    explicit Object(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Renderer> &renderer);

    static std::shared_ptr<Object> make(const std::shared_ptr<Mesh> &mesh, const std::string &shader);
    void setColor(const glm::vec3 &color);

    const std::array<glm::vec3, 8>& getBounds() const;

    void setRotation(float yaw, float pitch, float roll) override;

    bool shouldRender() const override;

    using RenderableEntity::setPosition;
    void setPosition(const glm::vec3& position) override;

    void setScale(const glm::vec3 &scale);

    [[nodiscard]] float getHeight() const;

    [[nodiscard]] glm::vec3 getColor() const;

    [[nodiscard]] glm::mat4 getModel() const override;

    std::shared_ptr<Mesh> getMesh() const;
};
#endif