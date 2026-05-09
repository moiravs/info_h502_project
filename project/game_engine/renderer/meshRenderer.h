
#ifndef INFOH502_CPP_MESHRENDERER_H
#define INFOH502_CPP_MESHRENDERER_H
#include "renderer.h"
#include <glm/gtx/string_cast.hpp>

class Object;
class DepthMap;

class MeshRenderer : public Renderer
{

protected:
    GLuint emptyTexture;
    void setupVAOs() override;

public:
    explicit MeshRenderer(const std::string &shaderName);
    explicit MeshRenderer(const std::shared_ptr<Shader>& shaderName);
    void registerEntity(const std::shared_ptr<RenderableEntity> &entity) override;

    void updateUniforms() const override;

    virtual void drawElements(int numTriangles);
    void renderDepth(const std::shared_ptr<DepthMap> &depthMap) const;

    void render() override;
};

#endif // INFOH502_CPP_MESHRENDERER_H
