
#include "lightManager.h"

#include "../../utils/utils.h"
#include "../entity/light/directionalLight.h"
#include "uboManager.h"

#include "../entity/light/pointLight.h"

LightManager::LightManager(): UboProvider("Lights", sizeof(LightBlock)) {}

LightManager& LightManager::get()
{
    static LightManager instance;
    return instance;
}

void LightManager::registerPointLight(std::shared_ptr<PointLight> light)
{
    if (this->lights.size() == MAX_LIGHTS)
    {
        ERROR("Maximal number of lights exceeded. Cannot add a new light to the scene.");
        return;
    }

    this->lights.push_back(std::move(light));
    this->notify();
}

void LightManager::registerDirectionalLight(const std::shared_ptr<DirectionalLight> &light)
{
    if (this->_sun)
    {
        FATAL("There is no support for multiple directional lights.");
    }

    this->_sun = light;
    this->notify();
}

void LightManager::notify()
{
    this->needsUpdate = true;
}

void LightManager::updateUBO()
{
    if (!this->needsUpdate) return;
    LightBlock g{};
    g.count = static_cast<int>(lights.size());
    g.sunPV = this->_sun->getPV();

    for (int i = 0; i < g.count; i++) {
        g.positions[i] = glm::vec4(lights[i]->getPosition(), 0);
        g.properties[i] = glm::vec4(lights[i]->getAmbient(), lights[i]->getDiffuse(),
            lights[i]->getSpecular(), lights[i]->getShininess());
        g.attenuations[i] = glm::vec4(lights[i]->getConstant(), lights[i]->getLinear(),
            lights[i]->getQuadratic(), 0);
        g.colors[i] = glm::vec4(lights[i]->getColor(), 0);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0,
                     sizeof(LightBlock),
                     &g);
    this->needsUpdate = false;
}