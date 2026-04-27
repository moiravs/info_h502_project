#include "objectRenderer.h"

#include "../../utils/utils.h"
#include "../manager/displaymanager.h"
#include "../entity/object.h"

ObjectRenderer::ObjectRenderer(const std::string& shaderName) : MeshRenderer(shaderName) {}

void ObjectRenderer::updateUniforms() const
{
    printMat4(this->_entity->getModel());
    this->_shader->setVector3f("materialColour", this->getEntity<Object>()->getColor());
    this->_shader->setMatrix4("model", this->_entity->getModel());
    const glm::mat4 inverseModel = glm::transpose(glm::inverse(this->_entity->getModel()));
    this->_shader->setMatrix4("itM", inverseModel);
}
