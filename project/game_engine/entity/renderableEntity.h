
#ifndef INFOH502_CPP_RENDERABLEENTITY_H
#define INFOH502_CPP_RENDERABLEENTITY_H
#include "entity.h"
#include "../renderable.h"
#include "../renderer/renderer.h"

class RenderableEntity : public Entity, public Renderable {
    std::shared_ptr<Renderer> _renderer;
    bool _dirty;
public:
    virtual glm::mat4 getModel() const;
    void dirty();
    explicit RenderableEntity(const std::shared_ptr<Renderer> &renderer);
    std::shared_ptr<Renderer> getRenderer();
    void setPosition(const glm::vec3 &position) override;
    void setRotation(float yaw, float pitch) override;
};



#endif //INFOH502_CPP_RENDERABLEENTITY_H
