
#include "lightManager.h"

#include "uboManager.h"

LightManager::LightManager()
{
    glGenBuffers(1, &ubo);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(CompactLight), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, UboManager::getBinding("Lights"), ubo);
}

LightManager::~LightManager()
{
    glDeleteBuffers(1, &ubo);
}

std::shared_ptr<LightManager> LightManager::instance = nullptr;

std::shared_ptr<LightManager> LightManager::get()
{
    if (instance == nullptr)
        instance = std::make_shared<LightManager>(LightManager());

    return instance;
}

void LightManager::registerLight(std::shared_ptr<Light> light)
{
    this->lights.push_back(std::move(light));
    this->notify();
}

void LightManager::notify() const
{
    // this gets called whenever a light changes
    this->rewriteBuffer();
}

void LightManager::rewriteBuffer() const
{
    std::vector<CompactLight> toCopy;
    toCopy.reserve(lights.size());

    for (const auto& l: lights) {
        CompactLight g{};

        g.position = l->getPosition();
        g.ambient_strength = l->getAmbient();
        g.diffuse_strength = l->getDiffuse();
        g.specular_strength = l->getSpecular();
        g.constant = l->getConstant();
        g.linear = l->getLinear();
        g.quadratic = l->getQuadratic();

        toCopy.push_back(g);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0,
                    toCopy.size() * sizeof(CompactLight),
                    toCopy.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
