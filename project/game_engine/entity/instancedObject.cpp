
#include "instancedObject.h"

#include "renderableEntityMaker.h"
#include "../renderer/instancedRenderer.h"
#include "../../utils/utils.h"

std::shared_ptr<InstancedObject> InstancedObject::make(const std::shared_ptr<Mesh> &mesh, const std::string& shader,
    const std::vector<glm::mat4>& models)
{
    auto r = RenderableEntityMaker::makeRenderable<InstancedObject, InstancedRenderer>(shader, mesh);
    r->setModels(models);
    return r;
}

InstancedObject::InstancedObject(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Renderer>& renderer)
: Object(mesh, renderer)
{}

std::shared_ptr<InstancedRenderer> InstancedObject::getInstancedRenderer() const
{
    const auto obj = std::dynamic_pointer_cast<InstancedRenderer>(this->getRenderer());
    if (!obj)
    {
        ERROR("The renderer linked to this object is not of type InstancedRenderer.");
    }
    return obj;
}

void InstancedObject::setModels(const std::vector<glm::mat4>& models)
{
    this->_models = models;
    this->getInstancedRenderer()->setInstanceMatrices(models);
}
