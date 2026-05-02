
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
    this->updateBounds();
}

void InstancedObject::updateBounds()
{
    if (this->_models.empty()) return;

    auto [meshMin, meshMax] = this->getMesh()->getBounds();

    const glm::vec3 corners[8] = {
        {meshMin.x, meshMin.y, meshMin.z},
        {meshMax.x, meshMin.y, meshMin.z},
        {meshMax.x, meshMax.y, meshMin.z},
        {meshMin.x, meshMax.y, meshMin.z},
        {meshMin.x, meshMin.y, meshMax.z},
        {meshMax.x, meshMin.y, meshMax.z},
        {meshMax.x, meshMax.y, meshMax.z},
        {meshMin.x, meshMax.y, meshMax.z}
    };

    glm::vec3 min(std::numeric_limits<float>::max());
    glm::vec3 max(-std::numeric_limits<float>::max());

    for (const auto& model : this->_models)
    {
        for (auto corner : corners)
        {
            const auto p = glm::vec3(model * glm::vec4(corner, 1.0));

            min = glm::min(min, p);
            max = glm::max(max, p);
        }
    }

    this->_bounds[0] = {min.x, min.y, min.z};
    this->_bounds[1] = {max.x, min.y, min.z};
    this->_bounds[2] = {max.x, max.y, min.z};
    this->_bounds[3] = {min.x, max.y, min.z};
    this->_bounds[4] = {min.x, min.y, max.z};
    this->_bounds[5] = {max.x, min.y, max.z};
    this->_bounds[6] = {max.x, max.y, max.z};
    this->_bounds[7] = {min.x, max.y, max.z};
}

