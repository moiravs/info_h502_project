#ifndef ObjectRenderer_H
#define ObjectRenderer_H

#include "meshRenderer.h"

class ObjectRenderer : public MeshRenderer
{
    bool _transparent{};
public:
    explicit ObjectRenderer(const std::string& shaderName);
    void updateUniforms() const override;
    void drawElements(int numTriangles) override;
};
#endif
