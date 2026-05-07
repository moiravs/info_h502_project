
#include "meshRenderer.h"

#include "../../utils/utils.h"
#include "../entity/object.h"

#include "../../utils/utils.h"
#include "../entity/object.h"
#include <glm/gtx/string_cast.hpp>

MeshRenderer::MeshRenderer(const std::string &shaderName) : Renderer(generateShader(shaderName))
{
    glGenTextures(1, &emptyTexture);
    glBindTexture(GL_TEXTURE_2D, emptyTexture);
    unsigned int transparent = 0x00000000; // Alpha is 00
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &transparent);
}

MeshRenderer::MeshRenderer(const std::shared_ptr<Shader> shader) : Renderer(shader)
{
    glGenTextures(1, &emptyTexture);
    glBindTexture(GL_TEXTURE_2D, emptyTexture);
    unsigned int transparent = 0x00000000; // Alpha is 00
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &transparent);
}

void MeshRenderer::updateUniforms() const
{
    _shader->use();
    _shader->setMatrix4("lightSpaceMatrix", this->lightSpaceMatrix);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, this->shadowMapTexture);
    _shader->setInteger("shadowMap", 4);

    glActiveTexture(GL_TEXTURE0);
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
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getEntries()[i].VB);

        const auto att_position = glGetAttribLocation(_shader->getID(), "position");
        if (att_position >= 0)
        {
            glEnableVertexAttribArray(att_position);

            glVertexAttribPointer(att_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, position)));
        }

        const auto att_tex = glGetAttribLocation(_shader->getID(), "tex_coord");
        if (att_tex >= 0)
        {
            glEnableVertexAttribArray(att_tex);
            glVertexAttribPointer(att_tex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, texture)));
        }

        const auto att_normal = glGetAttribLocation(_shader->getID(), "normal");
        if (att_normal >= 0)
        {
            glEnableVertexAttribArray(att_normal);

            glVertexAttribPointer(att_normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, normal)));
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getEntries()[i].IB);

        const auto att_model = glGetAttribLocation(_shader->getID(), "model");

        if (att_model >= 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, _VBOs[0]);
            for (unsigned int j = 0; j < 4; j++)
            {
                glEnableVertexAttribArray(att_model + j);
                glVertexAttribPointer(att_model + j, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void *)(sizeof(glm::vec4) * j));
                glVertexAttribDivisor(att_model + j, 1);
            }
        }
    }
    glBindVertexArray(0);
}

void MeshRenderer::renderShadows(const std::shared_ptr<Shader> &shadowShader) const
{
    shadowShader->use();
    const auto mesh = this->getEntity<Object>()->getMesh();

    for (unsigned int i = 0; i < _VAOs.size(); i++)
    {
        const auto &entry = mesh->getEntries()[i];

        shadowShader->setMatrix4("model", this->getEntity<Object>()->getModel());

        glBindVertexArray(_VAOs[i]);
        glDrawElements(GL_TRIANGLES, entry.numIndices, GL_UNSIGNED_INT, 0);
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
