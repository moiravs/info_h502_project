#ifndef ObjectRenderer_H
#define ObjectRenderer_H

#include "meshRenderer.h"

class ObjectRenderer : public MeshRenderer
{
    bool _transparent{};

public:
    explicit ObjectRenderer();
    void updateUniforms() const override;
    void drawElements(int numTriangles) override;
};
#endif
