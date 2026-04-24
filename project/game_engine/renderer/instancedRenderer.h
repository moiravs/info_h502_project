#ifndef InstancedRenderer_H
#define InstancedRenderer_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "meshRenderer.h"
#include "renderer.h"

class InstancedRenderer : public MeshRenderer
{
    unsigned int _instanceCount;

public:
    InstancedRenderer(const std::string& shaderName);
    void setInstanceMatrices(const std::vector<glm::mat4> &matrices);
    void updateUniforms() const override;
    void drawElements(int numTriangles) override;
};
#endif
