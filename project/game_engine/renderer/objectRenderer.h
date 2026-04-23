#ifndef ObjectRenderer_H
#define ObjectRenderer_H

#include <glad/glad.h>

#include "../entity/object.h"
#include "renderer.h"
#include <memory>
#include "../shader.h"

class ObjectRenderer : public Renderer
{
    bool _transparent{};
    std::vector<GLuint> _vaos;

public:
    explicit ObjectRenderer();
    void updateUniforms() const override;
    void registerObject(std::shared_ptr<Object> object) override;
    void render() override;
};
#endif
