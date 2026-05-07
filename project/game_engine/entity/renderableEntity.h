
#ifndef INFOH502_CPP_RENDERABLEENTITY_H
#define INFOH502_CPP_RENDERABLEENTITY_H
#include "entity.h"
#include "../renderable.h"

class Renderer;

class RenderableEntity : public Entity, public Renderable
{
    std::shared_ptr<Renderer> _renderer;
    bool _dirty;
public:
    virtual glm::mat4 getModel() const;
    void update(float delta) override;
    virtual bool shouldUpdate() const;
    virtual bool shouldRender() const;
    void dirty();
    [[nodiscard]] bool isDirty() const;
    explicit RenderableEntity(const std::shared_ptr<Renderer> &renderer);
    std::shared_ptr<Renderer> getRenderer() const;
    void render(float delta) override;
    void renderDepth() override;
    void setPosition(const glm::vec3 &position) override;
    void setRotation(float yaw, float pitch, float roll) override;
};

#endif // INFOH502_CPP_RENDERABLEENTITY_H
