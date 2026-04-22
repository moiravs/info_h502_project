#ifndef ObjectRenderer_H
#define ObjectRenderer_H

#include <glad/glad.h>

#include "../entity/object.h"
#include "renderer.h"
#include "../shader.h"

class ObjectRenderer : public Renderer
{
    bool _transparent{};
public:
    explicit ObjectRenderer(std::shared_ptr<Shader> shader);
    void registerObject(std::shared_ptr<Object> object) override;
    void render() override;
};
#endif
