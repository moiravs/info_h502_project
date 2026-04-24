#ifndef PARTICLE_H
#define PARTICLE_H

#include <glad/glad.h>
#include <glm/gtx/color_space.hpp>

#include <string>
#include <fstream>
#include <memory>
#include "shader.h"
#include "texture.h"
#include "entity/object.h"
#include "mainCamera.h"
// Represents a single particle and its state
struct Particle
{
    glm::vec3 pos, speed;
    glm::vec4 color;
    float life;
    float size;
    float cameraDist; // Squared distance to the camera position

    Particle() : pos(0.0f), speed(0.0f), color(1.0f), life(0.0f), size(0.0f), cameraDist(0.0f) {}

    bool operator<(const Particle &otherP) const
    {
        // sort in reverse order, first particles that are further away
        return this->cameraDist > otherP.cameraDist;
    }
};
// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
const int MaxParticles = 10000;
static GLfloat *g_particule_position_size_data = new GLfloat[MaxParticles * 4];
static GLfloat *g_particule_color_data = new GLfloat[MaxParticles * 4];
class ParticleGenerator
{
private:
    int particleCount = 0;
    Shader _shader;
    GLuint VBO_vertex, VBO_position, VBO_color, VAO;
    int lastUsedParticle = 0;
    Particle particlesContainer[MaxParticles];

public:
    void update(double delta, double currentTime);
    int findUnusedParticle();
    void sortParticles();
    void render();

    ParticleGenerator(Shader shader);
};

#endif
