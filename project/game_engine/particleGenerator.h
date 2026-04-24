#ifndef PARTICLE_H
#define PARTICLE_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <memory>
#include "shader.h"
#include "texture.h"
#include "entity/object.h"
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

class ParticleGenerator
{
public:
    int findUnusedParticle();
    void sortParticles();
    Particle particlesContainer[MaxParticles];

    int lastUsedParticle = 0;

    //     // constructor
    //     ParticleGenerator(Shader shader, Texture texture, unsigned int amount);
    //     // update all particles
    //     void Update(float dt, Object &object, unsigned int newParticles, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
    //     // render all particles
    //     void Draw();

    // private:
    //     // state
    //     std::vector<Particle> particles;
    //     unsigned int amount;
    //     // render state
    //     Shader shader;
    //     Texture texture;
    //     unsigned int VAO;
    //     // initializes buffer and vertex attributes
    //     void init();
    //     // returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    //     unsigned int firstUnusedParticle();
    //     // respawns particle
    //     void respawnParticle(Particle &particle, Object &object, glm::vec3 offset = glm::vec3(1.0f, 1.0f, 1.0f));
};

#endif
