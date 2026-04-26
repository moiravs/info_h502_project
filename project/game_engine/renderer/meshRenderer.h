
#ifndef INFOH502_CPP_MESHRENDERER_H
#define INFOH502_CPP_MESHRENDERER_H
#include "renderer.h"

class Object;

class MeshRenderer : public Renderer {
protected:
    void setupVAOs() override;
public:
    explicit MeshRenderer(const std::string& shaderName);
    void updateUniforms() const override;
    void registerEntity(const std::shared_ptr<RenderableEntity>& entity) override;
    virtual void drawElements(int numTriangles);
    void render() override;
};

#endif //INFOH502_CPP_MESHRENDERER_H
