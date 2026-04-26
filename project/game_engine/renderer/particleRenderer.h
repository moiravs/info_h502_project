#ifndef PARTICLE_H
#define PARTICLE_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "renderer.h"
#include "../../utils/constants.h"

class ParticleGenerator;

// Represents a single particle and its state
struct Particle
{
    glm::vec3 pos, speed;
    glm::vec4 color;
    float life;
    float size;
    float cameraDist; // Squared distance to the camera position
    float maxLife;

    Particle() : pos(0.0f), speed(0.0f), color(1.0f), life(0.0f), size(0.0f), cameraDist(0.0f), maxLife(0)
    {
    }

    bool operator<(const Particle &otherP) const
    {
        // sort in reverse order, first particles that are further away
        return this->cameraDist > otherP.cameraDist;
    }
};

// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.

class ParticleRenderer : public Renderer
{
protected:
    GLfloat *g_particule_position_size_data = new GLfloat[MAX_PARTICLES * 4];
    GLfloat *g_particule_color_data = new GLfloat[MAX_PARTICLES * 4];
    int particleCount = 0;

    int lastUsedParticle = 0;
    Particle particlesContainer[MAX_PARTICLES];

    void setupVAOs() override;
public:
    virtual void update(double delta);
    int findUnusedParticle();
    void sortParticles();
    void render() override;
    void updateUniforms() const override;
    std::shared_ptr<ParticleGenerator> getGenerator() const;

    explicit ParticleRenderer(const std::string &shader);
    ~ParticleRenderer() override;
};

#endif
