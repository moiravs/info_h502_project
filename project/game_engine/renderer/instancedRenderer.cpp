#include "instancedRenderer.h"

#include "../manager/displaymanager.h"
#include "../entity/object.h"

InstancedRenderer::InstancedRenderer(const std::string &shaderName) : MeshRenderer(shaderName), _instanceCount(0)
{
}

void InstancedRenderer::updateUniforms() const
{
    MeshRenderer::updateUniforms();
}

void InstancedRenderer::renderShadows(const std::shared_ptr<Shader> &shadowShader)
{
    shadowShader->use();
    const auto mesh = this->getEntity<Object>()->getMesh();
    shadowShader->setMatrix4("model", this->getEntity<Object>()->getModel());
    for (unsigned int i = 0; i < _VAOs.size(); i++)
    {
        glBindVertexArray(_VAOs[i]);

        glDrawElementsInstanced(
            GL_TRIANGLES,
            mesh->getEntries()[i].numIndices,
            GL_UNSIGNED_INT,
            0,
            _instanceCount);
    }
    glBindVertexArray(0);
}

void InstancedRenderer::setInstanceMatrices(const std::vector<glm::mat4> &matrices)
{
    _instanceCount = matrices.size();
    glBindBuffer(GL_ARRAY_BUFFER, _VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, _instanceCount * sizeof(glm::mat4), matrices.data(), GL_STATIC_DRAW);
}

void InstancedRenderer::drawElements(const int numTriangles)
{
    glDrawElementsInstanced(GL_TRIANGLES, numTriangles, GL_UNSIGNED_INT, nullptr, _instanceCount);
}
