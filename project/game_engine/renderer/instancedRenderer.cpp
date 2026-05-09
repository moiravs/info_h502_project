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

void InstancedRenderer::setupVAOs()
{
    if (!this->getEntity<Object>() || !this->getEntity<Object>()->getMesh())
        return;

    const auto& mesh = this->getEntity<Object>()->getMesh();

    this->createVAOs(mesh->getEntries().size());
    this->createVBOs(1);

    for (unsigned int i = 0; i < mesh->getEntries().size(); i++)
    {
        glBindVertexArray(_VAOs[i]);

        // Bind vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getEntries()[i].VB);

        // =========================
        // POSITION (location = 0)
        // =========================
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, position)
        );

        // =========================
        // TEXCOORD (location = 1)
        // =========================
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, texture)
        );

        // =========================
        // NORMAL (location = 2)
        // =========================
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            2,
            3,
            GL_FLOAT,
            GL_FALSE,
            sizeof(Vertex),
            (void*)offsetof(Vertex, normal)
        );

        // =========================
        // INDEX BUFFER
        // =========================
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getEntries()[i].IB);

        // =========================
        // MODEL MATRIX (instancing, location = 3,4,5,6)
        // =========================
        glBindBuffer(GL_ARRAY_BUFFER, _VBOs[0]);

        for (unsigned int j = 0; j < 4; j++)
        {
            glEnableVertexAttribArray(3 + j);
            glVertexAttribPointer(
                3 + j,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(glm::mat4),
                (void*)(sizeof(glm::vec4) * j)
            );
            glVertexAttribDivisor(3 + j, 1);
        }
    }

    glBindVertexArray(0);
}
