
#include "lightManager.h"

LightManager::LightManager()
{
    glGenBuffers(1, &ubo);
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

void LightManager::registerShader(std::shared_ptr<Shader> shader)
{
    // TODO
}

void LightManager::rewriteBuffer() const
{
    std::vector<GPULight> toCopy;
    toCopy.reserve(lights.size());

    for (const auto& l: lights) {
        GPULight g{};

        g.position = l->getPosition();
        g.ambient_strength = l->getAmbient();
        g.diffuse_strength = l->getDiffuse();
        g.specular_strength = l->getSpecular();
        g.constant = l->getConstant();
        g.linear = l->getLinear();
        g.quadratic = l->getQuadratic();

        toCopy.push_back(g);
    }

    //TODO not sure it's correct

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0,
                    toCopy.size() * sizeof(GPULight),
                    toCopy.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
