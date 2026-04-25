
#include "meshRenderer.h"

MeshRenderer::MeshRenderer(const std::string &shaderName) : Renderer(generateShader(shaderName))
{
    glGenTextures(1, &emptyTexture);
    glBindTexture(GL_TEXTURE_2D, emptyTexture);
    unsigned int transparent = 0x00000000; // Alpha is 00
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &transparent);
}

void MeshRenderer::updateUniforms() const
{
}

void MeshRenderer::registerObject(const std::shared_ptr<Object> object)
{
    Renderer::registerObject(object);

    if (!this->_object->getMesh())
        return;
    this->setupVAOs();
}

void MeshRenderer::setupVAOs()
{
    if (!this->_object || !this->_object->getMesh())
        return;

    const auto &mesh = this->_object->getMesh();
    this->createVAOs(mesh->m_Entries.size());

    for (unsigned int i = 0; i < mesh->m_Entries.size(); i++)
    {
        glBindVertexArray(_VAOs[i]);
        glBindBuffer(GL_ARRAY_BUFFER, mesh->m_Entries[i].VB);

        const auto att_position = glGetAttribLocation(_shader->getID(), "position");
        if (att_position >= 0)
        {
            glEnableVertexAttribArray(att_position);
            glVertexAttribPointer(att_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Position)));
        }

        const auto att_tex = glGetAttribLocation(_shader->getID(), "tex_coord");
        if (att_tex >= 0)
        {
            glEnableVertexAttribArray(att_tex);
            glVertexAttribPointer(att_tex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Texture)));
        }

        const auto att_normal = glGetAttribLocation(_shader->getID(), "normal");
        if (att_normal >= 0)
        {
            glEnableVertexAttribArray(att_normal);
            glVertexAttribPointer(att_normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, Normal)));
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

    if (!_object->getMesh())
        return;
    const auto mesh = _object->getMesh();

    for (unsigned int i = 0; i < _VAOs.size(); i++)
    {
        const auto &entry = mesh->m_Entries[i];

        // Use texture from Assimp material, fallback to constructor texture if null
        if (entry.MaterialIndex < mesh->m_Textures.size() && mesh->m_Textures[entry.MaterialIndex])
        {
            mesh->m_Textures[entry.MaterialIndex]->bind();
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, emptyTexture); // Bind the "Invisible" texture
        }

        glBindVertexArray(_VAOs[i]);
        this->drawElements(entry.NumIndices);
    }

    glBindVertexArray(0);
}
