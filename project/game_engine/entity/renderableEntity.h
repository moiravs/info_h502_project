
#ifndef INFOH502_CPP_RENDERABLEENTITY_H
#define INFOH502_CPP_RENDERABLEENTITY_H
#include "entity.h"
#include "../renderable.h"
#include "../manager/octreeManager.h"

class Renderer;
class Octree;

class RenderableEntity : public Entity, public Renderable
{
    std::shared_ptr<Renderer> _renderer;
    std::shared_ptr<Octree> octreeNode = nullptr;
    bool _dirty;

public:
    void setOctreeNode(const std::shared_ptr<Octree> &node);
    virtual glm::mat4 getModel() const;
    void update(float delta) override;
    virtual bool shouldUpdate() const;
    void dirty();
    [[nodiscard]] bool isDirty() const;
    explicit RenderableEntity(const std::shared_ptr<Renderer> &renderer);
    std::shared_ptr<Renderer> getRenderer() const;
    void render(float delta) override;
    void setPosition(const glm::vec3 &position) override;
    void setRotation(float yaw, float pitch, float roll) override;
};

#endif // INFOH502_CPP_RENDERABLEENTITY_H
