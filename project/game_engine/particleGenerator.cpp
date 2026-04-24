

#include "particleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader shader) : _shader(shader)
{
    const float vertexData[18] = {
        // vertices
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0,
        -1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        -1.0, 1.0, 0.0,
        1.0, -1.0, 0.0};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_vertex);
    glGenBuffers(1, &VBO_position);
    glGenBuffers(1, &VBO_color);

    // define VBO and VAO as active buffer and active vertex array
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    auto att_vertex = glGetAttribLocation(shader.getID(), "vertex");
    glEnableVertexAttribArray(att_vertex);
    glVertexAttribPointer(att_vertex, 3, GL_FLOAT, false, 0, 0);
    glVertexAttribDivisor(att_vertex, 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW);

    auto att_center = glGetAttribLocation(shader.getID(), "center");
    glEnableVertexAttribArray(att_center);
    glVertexAttribPointer(att_center, 4, GL_FLOAT, false, 0, 0);
    glVertexAttribDivisor(att_center, 1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    auto att_col = glGetAttribLocation(shader.getID(), "col");
    glEnableVertexAttribArray(att_col);
    glVertexAttribPointer(att_col, 4, GL_FLOAT, true, 0, 0);
    glVertexAttribDivisor(att_col, 1);

    // desactive the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleGenerator::update(double delta, double currentTime)
{
    particleCount = 0;
    int newParticle = delta * 1000.0f;
    if (newParticle > (int)(0.032f * 1000.0))
        newParticle = (int)(0.032f * 1000.0);

    for (int i = 0; i < newParticle; i++)
    {
        int particleIdx = this->findUnusedParticle();
        this->particlesContainer[particleIdx].life = 15.0f + rand() % 10 / 10.0;
        this->particlesContainer[particleIdx].pos = glm::vec3(glm::cos(currentTime) * 0.5, 2.0 + rand() % 100 / 1000.0, -5.0f + rand() % 100 / 1000.0);
        this->particlesContainer[particleIdx].speed = glm::vec3(rand() % 20 / 30.0, 1.0 - rand() % 10 / 20.0, rand() % 100 / 1000.0);

        // use hsv color to get pretty results
        this->particlesContainer[particleIdx].color = glm::vec4(209.0, 0.07, 0.89, 0.5);

        this->particlesContainer[particleIdx].size = 0.05f;
    }

    // Simulate the particle
    for (int i = 0; i < MaxParticles; i++)
    {
        Particle &p = this->particlesContainer[i]; // shortcut

        if (p.life > 0.0)
        {
            // decrease life, use time since last frame
            p.life -= delta;

            // change of pos
            p.pos += p.speed * (float)delta;

            p.color.y += (float)delta * 0.5;
            p.color.r += glm::cos(rand()) * 0.5;

            p.color.a -= (float)delta * 0.05;

            const auto camera = MainCamera::get();

            // update distance with the camera
            p.cameraDist = glm::length(p.pos - camera->getPosition());

            // fill the gpu buffer
            g_particule_position_size_data[4 * particleCount] = p.pos.x;
            g_particule_position_size_data[4 * particleCount + 1] = p.pos.y;
            g_particule_position_size_data[4 * particleCount + 2] = p.pos.z;

            g_particule_position_size_data[4 * particleCount + 3] = p.size;

            glm::vec3 hsv = glm::vec3(p.color.r, p.color.g, p.color.b);
            glm::vec3 rgb = glm::rgbColor(hsv);
            g_particule_color_data[4 * particleCount + 0] = rgb.r;
            g_particule_color_data[4 * particleCount + 1] = rgb.g;
            g_particule_color_data[4 * particleCount + 2] = rgb.b;
            g_particule_color_data[4 * particleCount + 3] = p.color.a;

            particleCount++;
        }

        else
        {
            // make sure all dead particle will be put at the end of the list
            p.cameraDist = -1;
        }
    }
}

void ParticleGenerator::render()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW);
    ;
    glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    ;
    glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLfloat) * 4, g_particule_color_data);

    _shader.use();

    const auto camera = MainCamera::get();

    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 perspective = camera->getProjectionMatrix();

    glm::vec3 cameraRight = camera->getRight();
    glm::vec3 cameraUp = camera->getUp();
    glm::vec3 cameraPosition = camera->getPosition();

    view = camera->getViewMatrix();
    perspective = camera->getProjectionMatrix(45.0, 800 / 600, 0.01, 100.0);
    cameraRight = camera->getRight();
    cameraUp = camera->getUp();
    cameraPosition = camera->getPosition();

    _shader.setMatrix4("V", view);
    _shader.setMatrix4("P", perspective);

    _shader.setVector3f("cameraRight", cameraRight);
    _shader.setVector3f("cameraUp", cameraUp);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, particleCount);
}

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