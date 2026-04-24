#ifndef ObjectRenderer_H
#define ObjectRenderer_H

#include "../entity/object.h"
#include "renderer.h"
#include "../shader.h"

class ObjectRenderer : public Renderer
{
    bool _transparent{};
protected:
    [[nodiscard]] std::string getShaderName() const override;
public:
    explicit ObjectRenderer();
    void updateUniforms() const override;
    void registerObject(std::shared_ptr<Object> object) override;
    void render() override;
};
#endif
