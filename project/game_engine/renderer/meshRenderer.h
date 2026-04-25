
#ifndef INFOH502_CPP_MESHRENDERER_H
#define INFOH502_CPP_MESHRENDERER_H
#include "renderer.h"


class MeshRenderer : public Renderer {
protected:
    void setupVAOs() override;
    std::shared_ptr<Object> getObject() const;
public:
    explicit MeshRenderer(const std::string& shaderName);
    void updateUniforms() const override;
    virtual void drawElements(int numTriangles)=0;
    void render() override;
};

#endif //INFOH502_CPP_MESHRENDERER_H
