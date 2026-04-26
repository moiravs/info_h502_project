
#include "particleGenerator.h"

#include "renderableEntityMaker.h"
#include "../renderer/particleRenderer.h"
#include "../../utils/utils.h"

std::shared_ptr<ParticleGenerator> ParticleGenerator::make(const ParticleParams& params)
{
    return RenderableEntityMaker::makeRenderable<ParticleGenerator, ParticleRenderer>("part", params);
}

ParticleGenerator::ParticleGenerator(const ParticleParams& params, const std::shared_ptr<ParticleRenderer>& renderer)
    : RenderableEntity(renderer), _params(params)
{}

const ParticleParams& ParticleGenerator::getParams() const
{
    return this->_params;
}

void ParticleGenerator::update(const float delta)
{
    this->getParticleRenderer()->update(delta);
}

bool ParticleGenerator::shouldUpdate() const
{
    return true;
}

std::shared_ptr<ParticleRenderer> ParticleGenerator::getParticleRenderer() const
{
    const auto obj = std::dynamic_pointer_cast<ParticleRenderer>(this->getRenderer());
    if (!obj)
    {
        ERROR("The renderer linked to this object is not of type InstancedRenderer.");
    }
    return obj;
}
