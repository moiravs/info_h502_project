#include "waterRenderer.h"

WaterRenderer::WaterRenderer(std::shared_ptr<WaterFrameBuffer> fbos)
    : MeshRenderer("water"), transparent(false), _fbos(std::move(fbos))
{}

void WaterRenderer::registerEntity(const std::shared_ptr<RenderableEntity>& entity)
{
    this->MeshRenderer::registerEntity(entity);

    this->_shader->setInteger("reflectionTexture", 0);
    this->_shader->setInteger("refractionTexture", 1);
    this->_shader->setInteger("dudvMap", 2);
}

void WaterRenderer::setupVAOs()
{
    // define VBO and VAO as active buffer and active vertex array
    const auto &mesh = this->getObject()->getMesh();

    if (mesh->m_Entries.empty())
        return;

    this->createVAOs(1);
    this->createVBOs(1);

    glBindVertexArray(_VAOs[0]);

    // USE THE BUFFER FROM THE MESH ENTRY, NOT _VBOs[0]
    glBindBuffer(GL_ARRAY_BUFFER, mesh->m_Entries[0].VB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_Entries[0].IB);

    auto att_pos = glGetAttribLocation(_shader->getID(), "position");
    if (att_pos >= 0)
    {
        glEnableVertexAttribArray(att_pos);
        glVertexAttribPointer(att_pos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Position));
    }

    auto att_nor = glGetAttribLocation(_shader->getID(), "normal");
    if (att_nor >= 0)
    {
        glEnableVertexAttribArray(att_nor);
        glVertexAttribPointer(att_nor, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Normal));
    }

    auto att_tex = glGetAttribLocation(_shader->getID(), "tex_coord");
    if (att_tex >= 0)
    {
        glEnableVertexAttribArray(att_tex);
        glVertexAttribPointer(att_tex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Texture));
    }
    // desactive the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void WaterRenderer::render()
{
    if (!this->getObject() || !this->getObject()->getMesh() || this->getObject()->getMesh()->m_Entries.empty())
        return;

    Renderer::render();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _fbos->getReflectionTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _fbos->getRefractionTexture());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dudvMap.getTexture());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(_VAOs[0]);

    const auto &entry = this->getObject()->getMesh()->m_Entries[0];

    glDrawElements(GL_TRIANGLES, entry.NumIndices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void WaterRenderer::updateUniforms() const
{
}
