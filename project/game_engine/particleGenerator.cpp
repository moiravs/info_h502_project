

#include "particleGenerator.h"


int ParticleGenerator::findUnusedParticle()
{
    for (int i = lastUsedParticle; i < MaxParticles; i++)
    {
        if (particlesContainer[i].life < 0)
        {
            lastUsedParticle = i;
            return i;
        }
    }

    for (int i = 0; i < lastUsedParticle; i++)
    {
        if (particlesContainer[i].life < 0)
        {
            lastUsedParticle = i;
            return i;
        }
    }

    return 0;
}

void ParticleGenerator::sortParticles()
{
    std::sort(&particlesContainer[0], &particlesContainer[MaxParticles]);
}