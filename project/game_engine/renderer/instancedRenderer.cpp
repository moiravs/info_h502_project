#include "instancedRenderer.h"

#include "../manager/displaymanager.h"
#include "../entity/object.h"

InstancedRenderer::InstancedRenderer()
    : MeshRenderer(this->generateShader("tree"))
{
    this->createVBOs(1);
}


void InstancedRenderer::updateUniforms() const
{
}

void InstancedRenderer::setInstanceMatrices(const std::vector<glm::mat4>& matrices)
{
    _instanceCount = matrices.size();
    glBindBuffer(GL_ARRAY_BUFFER, _VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, _instanceCount * sizeof(glm::mat4), matrices.data(), GL_STATIC_DRAW);
}

void InstancedRenderer::drawElements(const int numTriangles)
{
    glDrawElementsInstanced(GL_TRIANGLES, numTriangles, GL_UNSIGNED_INT, nullptr, _instanceCount);
}
