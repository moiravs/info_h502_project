
#include "meshRenderer.h"

#include "../../utils/utils.h"
#include "../entity/object.h"

#include "../../utils/utils.h"
#include "../entity/object.h"
#include <glm/gtx/string_cast.hpp>

#include "../depth/depthMapFrameBuffer.h"
#include "objectRenderer.h"

MeshRenderer::MeshRenderer(const std::string &shaderName) : Renderer(generateShader(shaderName))
{
    glGenTextures(1, &emptyTexture);
    glBindTexture(GL_TEXTURE_2D, emptyTexture);
    constexpr unsigned int transparent = 0x00000000;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &transparent);
}

MeshRenderer::MeshRenderer(const std::shared_ptr<Shader> &shaderName) : Renderer(shaderName)
{
    glGenTextures(1, &emptyTexture);
    glBindTexture(GL_TEXTURE_2D, emptyTexture);
    constexpr unsigned int transparent = 0x00000000; // Alpha is 00
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &transparent);
}

void MeshRenderer::registerEntity(const std::shared_ptr<RenderableEntity> &entity)
{
    this->Renderer::registerEntity(entity);
}

void MeshRenderer::setupVAOs()
{
    if (!this->getEntity<Object>() || !this->getEntity<Object>()->getMesh())
        return;

    const auto &mesh = this->getEntity<Object>()->getMesh();

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
            (void *)offsetof(Vertex, position));

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
            (void *)offsetof(Vertex, texture));

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
            (void *)offsetof(Vertex, normal));

        // =========================
        // INDEX BUFFER
        // =========================
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getEntries()[i].IB);
    }

    glBindVertexArray(0);
}

void MeshRenderer::renderWithShader(const std::shared_ptr<Shader> &shader)
{
    shader->use();
    const auto mesh = this->getEntity<Object>()->getMesh();

    for (unsigned int i = 0; i < _VAOs.size(); i++)
    {
        const auto &entry = mesh->getEntries()[i];

        shader->setMatrix4("model", this->getEntity<Object>()->getModel());

        glBindVertexArray(_VAOs[i]);
        this->drawElements(entry.numIndices);
    }
    glBindVertexArray(0);
}

void MeshRenderer::render()
{
    Renderer::render(); // Updates shader uniforms

    const auto mesh = this->getEntity<Object>()->getMesh();

    for (unsigned int i = 0; i < _VAOs.size(); i++)
    {
        const auto &entry = mesh->getEntries()[i];

        // Use texture from Assimp material, fallback to constructor texture if null
        if (entry.materialIndex < mesh->m_Textures.size() && mesh->m_Textures[entry.materialIndex])
        {
            mesh->m_Textures[entry.materialIndex]->bind();
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, emptyTexture); // Bind the "Invisible" texture
        }

        glBindVertexArray(_VAOs[i]);
        this->drawElements(entry.numIndices);
    }

    glBindVertexArray(0);
}

void MeshRenderer::drawElements(const int numTriangles)
{
    glDrawElements(GL_TRIANGLES, numTriangles, GL_UNSIGNED_INT, 0);
}

void MeshRenderer::updateUniforms() const {}
