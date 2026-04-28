#include "waterRenderer.h"

WaterRenderer::WaterRenderer(std::shared_ptr<WaterFrameBuffer> fbos)
    : MeshRenderer("water"), transparent(false), _fbos(std::move(fbos))
{
}

void WaterRenderer::registerEntity(const std::shared_ptr<RenderableEntity> &entity)
{
    this->MeshRenderer::registerEntity(entity);

    this->_shader->setInteger("reflectionTexture", 0);
    this->_shader->setInteger("refractionTexture", 1);
    this->_shader->setInteger("dudvMap", 2);
}

void WaterRenderer::render()
{
    const auto o = this->getEntity<Object>();
    if (!o || !o->getMesh() || o->getMesh()->getEntries().empty())
        return;

    Renderer::render();

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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _fbos->getReflectionTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _fbos->getRefractionTexture());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, dudvMap.getTexture());
}