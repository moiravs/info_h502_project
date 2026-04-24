

#include "particleGenerator.h"

ParticleGenerator::ParticleGenerator(const std::shared_ptr<Shader> shader) : _shader(shader)
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

    auto att_vertex = glGetAttribLocation(shader->getID(), "vertex");
    glEnableVertexAttribArray(att_vertex);
    glVertexAttribPointer(att_vertex, 3, GL_FLOAT, false, 0, 0);
    glVertexAttribDivisor(att_vertex, 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GL_FLOAT), NULL, GL_STREAM_DRAW);

    auto att_center = glGetAttribLocation(shader->getID(), "center");
    glEnableVertexAttribArray(att_center);
    glVertexAttribPointer(att_center, 4, GL_FLOAT, false, 0, 0);
    glVertexAttribDivisor(att_center, 1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

    auto att_col = glGetAttribLocation(shader->getID(), "col");
    glEnableVertexAttribArray(att_col);
    glVertexAttribPointer(att_col, 4, GL_FLOAT, true, 0, 0);
    glVertexAttribDivisor(att_col, 1);

    // desactive the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    for (int i = 0; i < MaxParticles; i++)
    {
        this->particlesContainer[i].life = -1.0;
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

    _shader->use();

    const auto camera = MainCamera::get();

    glm::vec3 cameraRight = camera->getRight();
    glm::vec3 cameraUp = camera->getUp();

    cameraRight = camera->getRight();
    cameraUp = camera->getUp();

    _shader->setVector3f("cameraRight", cameraRight);
    _shader->setVector3f("cameraUp", cameraUp);

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