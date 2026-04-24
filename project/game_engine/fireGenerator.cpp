

#include "fireGenerator.h"

FireGenerator::FireGenerator(const std::shared_ptr<Shader> shader, int height) : ParticleGenerator(shader), _height(height)
{
}

void FireGenerator::update(double delta, double currentTime)
{
    particleCount = 0;
    int newParticle = delta * 1000.0f;
    if (newParticle > (int)(0.032f * 1000.0))
        newParticle = (int)(0.032f * 1000.0);

    for (int i = 0; i < newParticle; i++)
    {
        int particleIdx = this->findUnusedParticle();
        float initialLife = 1.0f + (rand() % 100 / 100.0f);

        this->particlesContainer[particleIdx].life = initialLife;
        this->particlesContainer[particleIdx].maxLife = initialLife;
        this->particlesContainer[particleIdx].pos = glm::vec3(0.5, this->_height, -5.0f);

        float spread = 0.2f;
        float range = 0.5f;
        float offsetX = ((rand() % 1000 / 1000.0f) - 0.5f) * range;
        float offsetZ = ((rand() % 1000 / 1000.0f) - 0.5f) * range;

        this->particlesContainer[particleIdx].pos = glm::vec3(0.5f + offsetX, this->_height, -5.0f + offsetZ);

        float vx = ((rand() % 100 - 50) / 250.0f);
        float vz = ((rand() % 100 - 50) / 250.0f);
        float vy = 1.0f + (rand() % 100 / 100.0f);
        this->particlesContainer[particleIdx].speed = glm::vec3(vx, vy, vz);

        this->particlesContainer[particleIdx].size = 0.1f;

        this->particlesContainer[particleIdx].color = glm::vec4(1.0f, 0.9f, 0.3f, 1.0f);
        this->particlesContainer[particleIdx].size = 0.005f;
    }

    for (int i = 0; i < MaxParticles; i++)
    {
        Particle &p = this->particlesContainer[i];
        if (p.life > 0.0)
        {
            p.life -= delta;
            p.pos += p.speed * (float)delta;

            float lifeRatio = p.life / p.maxLife;

            glm::vec3 colorHot = glm::vec3(1.0f, 1.0f, 0.8f);
            glm::vec3 colorMid = glm::vec3(1.0f, 0.5f, 0.0f);
            glm::vec3 colorEnd = glm::vec3(0.5f, 0.0f, 0.0f);

            glm::vec3 finalRGB;
            if (lifeRatio > 0.5f)
            {
                finalRGB = glm::mix(colorMid, colorHot, (lifeRatio - 0.5f) * 2.0f);
            }
            else
            {
                finalRGB = glm::mix(colorEnd, colorMid, lifeRatio * 2.0f);
            }

            p.size = 0.1f * lifeRatio;

            float alpha = lifeRatio;

            g_particule_position_size_data[4 * particleCount + 0] = p.pos.x;
            g_particule_position_size_data[4 * particleCount + 1] = p.pos.y;
            g_particule_position_size_data[4 * particleCount + 2] = p.pos.z;
            g_particule_position_size_data[4 * particleCount + 3] = p.size;

            g_particule_color_data[4 * particleCount + 0] = finalRGB.r;
            g_particule_color_data[4 * particleCount + 1] = finalRGB.g;
            g_particule_color_data[4 * particleCount + 2] = finalRGB.b;
            g_particule_color_data[4 * particleCount + 3] = alpha;

            particleCount++;
        }

        else
        {
            // make sure all dead particle will be put at the end of the list
            p.cameraDist = -1;
        }
    }

    this->sortParticles();
}
