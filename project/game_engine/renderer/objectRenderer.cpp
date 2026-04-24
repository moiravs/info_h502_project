#include "objectRenderer.h"

#include "../manager/displaymanager.h"

ObjectRenderer::ObjectRenderer(const std::string& shaderName) : MeshRenderer(shaderName) {}

void ObjectRenderer::updateUniforms() const
{
    this->_shader->setVector3f("materialColour", this->_object->getColor());
    this->_shader->setMatrix4("model", this->_object->getModel());
    const glm::mat4 inverseModel = glm::transpose(glm::inverse(this->_object->getModel()));
    this->_shader->setMatrix4("itM", inverseModel);
}

void ObjectRenderer::drawElements(const int numTriangles)
{
    glDrawElements(GL_TRIANGLES, numTriangles, GL_UNSIGNED_INT, 0);

}
