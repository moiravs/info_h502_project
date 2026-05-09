#include "waterRenderer.h"

WaterRenderer::WaterRenderer()
    : MeshRenderer("water"), transparent(false)
{
}

void WaterRenderer::registerEntity(const std::shared_ptr<RenderableEntity> &entity)
{
    this->MeshRenderer::registerEntity(entity);

    this->_shader->use();
    this->_shader->setInteger("reflectionTexture", 5);
    this->_shader->setInteger("refractionTexture", 6);
    this->_shader->setInteger("dudvMap", 7);
}

void WaterRenderer::render()
{
    this->_shader->setMatrix4("model", this->_entity->getModel());
    const auto o = this->getEntity<Object>();
    if (!o || !o->getMesh() || o->getMesh()->getEntries().empty())
        return;

    Renderer::render();

    this->moveFactor += WAVE_SPEED;

    this->moveFactor = std::fmod(this->moveFactor, 1);
    this->_shader->setFloat("moveFactor", moveFactor);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(_VAOs[0]);

    const auto &entry = o->getMesh()->getEntries()[0];

    glDrawElements(GL_TRIANGLES, entry.numIndices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glDisable(GL_BLEND);
}

void WaterRenderer::updateUniforms() const
{
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, dudvMap.getTexture());
}