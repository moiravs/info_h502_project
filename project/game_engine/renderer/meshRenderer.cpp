
#include "meshRenderer.h"

#include "../../utils/utils.h"
#include "../entity/object.h"

MeshRenderer::MeshRenderer(const std::string& shaderName): Renderer(generateShader(shaderName)) {}

void MeshRenderer::updateUniforms() const
{}

void MeshRenderer::registerEntity(const std::shared_ptr<RenderableEntity>& entity)
{
    this->Renderer::registerEntity(entity);
}

std::shared_ptr<Object> MeshRenderer::getObject() const
{
    if (!this->_entity)
    {
        ERROR("There is no linked entity.");
    }
    const auto obj = std::dynamic_pointer_cast<Object>(this->_entity);
    if (!obj)
    {
        ERROR("The entity linked to this renderer is not of the correct type");
    }
    return obj;
}

void MeshRenderer::setupVAOs()
{
    if (!this->getObject() || !this->getObject()->getMesh())
        return;

    const auto &mesh = this->getObject()->getMesh();
    this->createVAOs(mesh->m_Entries.size());
    this->createVBOs(1);

    for (unsigned int i = 0; i < mesh->m_Entries.size(); i++)
    {
        glBindVertexArray(_VAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->m_Entries[i].VB);

        const auto att_position = glGetAttribLocation(_shader->getID(), "position");
        if (att_position >= 0)
        {
            glEnableVertexAttribArray(att_position);
            glVertexAttribPointer(att_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Position)));
        }

        const auto att_tex = glGetAttribLocation(_shader->getID(), "tex_coord");
        if (att_tex >= 0)
        {
            glEnableVertexAttribArray(att_tex);
            glVertexAttribPointer(att_tex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Texture)));
        }

        const auto att_normal = glGetAttribLocation(_shader->getID(), "normal");
        if (att_normal >= 0)
        {
            glEnableVertexAttribArray(att_normal);
            glVertexAttribPointer(att_normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, Normal)));
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_Entries[i].IB);

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

void MeshRenderer::render()
{
    Renderer::render(); // Updates shader uniforms

    if (!getObject()->getMesh()) return;
    const auto mesh = getObject()->getMesh();

    for (unsigned int i = 0; i < _VAOs.size(); i++)
    {
        const auto &entry = mesh->m_Entries[i];

        // Use texture from Assimp material, fallback to constructor texture if null
        if (entry.MaterialIndex < mesh->m_Textures.size() && mesh->m_Textures[entry.MaterialIndex])
        {
            mesh->m_Textures[entry.MaterialIndex]->bind();
        }

        glBindVertexArray(_VAOs[i]);
        this->drawElements(entry.NumIndices);
    }

    glBindVertexArray(0);
}
