
#include "lightManager.h"

#include "uboManager.h"
#include "../../utils/utils.h"

LightManager::LightManager()
{
    glGenBuffers(1, &ubo);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(LightBlock), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferBase(GL_UNIFORM_BUFFER, UboManager::get().getBinding("Lights"), ubo);
}

LightManager::~LightManager()
{
    glDeleteBuffers(1, &ubo);
}

LightManager& LightManager::get()
{
    static LightManager instance;
    return instance;
}

void LightManager::registerLight(std::shared_ptr<Light> light)
{
    if (lights.size() == MAX_LIGHTS)
    {
        ERROR("Maximal number of lights exceeded. Cannot add a new light to the scene.");
        return;
    }

    this->lights.push_back(std::move(light));
    this->notify();
}

void LightManager::notify()
{
    this->needsUpdate = true;
}

void LightManager::applyChanges()
{
    if (!this->needsUpdate) return;
    this->rewriteBuffer();
    this->needsUpdate = false;
}

void LightManager::rewriteBuffer() const
{
    LightBlock g{};
    g.count = static_cast<int>(lights.size());

    for (int i = 0; i < g.count; i++) {
        g.positions[i] = glm::vec4(lights[i]->getPosition(), 0);
        g.properties[i] = glm::vec4(lights[i]->getAmbient(), lights[i]->getDiffuse(),
            lights[i]->getSpecular(), lights[i]->getShininess());
        g.attenuations[i] = glm::vec4(lights[i]->getConstant(), lights[i]->getLinear(),
            lights[i]->getQuadratic(), 0);
    }

    // g.positions[0] = glm::vec4(1, 1, 1, 1);

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0,
                     sizeof(LightBlock),
                     &g);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
