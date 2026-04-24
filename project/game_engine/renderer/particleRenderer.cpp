

#include "particleRenderer.h"

#include "../../utils/utils.h"

ParticleRenderer::ParticleRenderer(const ParticleParams& params) : Renderer(generateShader("part")), params(params)
{
    constexpr float vertexData[18] = {
        // vertices
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0,
        -1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        -1.0, 1.0, 0.0,
        1.0, -1.0, 0.0
    };

    glGenBuffers(1, &VBO_vertex);
    glGenBuffers(1, &VBO_position);
    glGenBuffers(1, &VBO_color);

    // define VBO and VAO as active buffer and active vertex array
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    auto att_vertex = glGetAttribLocation(_shader->getID(), "vertex");
    glEnableVertexAttribArray(att_vertex);
    glVertexAttribPointer(att_vertex, 3, GL_FLOAT, false, 0, 0);
    glVertexAttribDivisor(att_vertex, 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GL_FLOAT), nullptr, GL_STREAM_DRAW);

    auto att_center = glGetAttribLocation(_shader->getID(), "center");
    glEnableVertexAttribArray(att_center);
    glVertexAttribPointer(att_center, 4, GL_FLOAT, false, 0, 0);
    glVertexAttribDivisor(att_center, 1);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), nullptr, GL_STREAM_DRAW);

    auto att_col = glGetAttribLocation(_shader->getID(), "col");
    glEnableVertexAttribArray(att_col);
    glVertexAttribPointer(att_col, 4, GL_FLOAT, true, 0, 0);
    glVertexAttribDivisor(att_col, 1);

    // desactive the buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        this->particlesContainer[i].life = -1.0;
    }
}

void ParticleRenderer::updateUniforms() const
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GL_FLOAT), nullptr, GL_STREAM_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_color);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLfloat), nullptr, GL_STREAM_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(GLfloat) * 4, g_particule_color_data);
}

void ParticleRenderer::render()
{
    Renderer::render();

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 6, particleCount);
}

int ParticleRenderer::findUnusedParticle()
{
    for (int i = lastUsedParticle; i < MAX_PARTICLES; i++)
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

void ParticleRenderer::sortParticles()
{
    std::sort(&particlesContainer[0], &particlesContainer[MAX_PARTICLES]);
}

void ParticleRenderer::update(const double delta)
{
    particleCount = 0;
    int newParticle = delta * 1000.0f;
    if (newParticle > static_cast<int>(0.032f * 1000.0))
        newParticle = static_cast<int>(0.032f * 1000.0);

    for (int i = 0; i < newParticle; i++)
    {
        const int particleIdx = this->findUnusedParticle();
        const float initialLife = params.maxLife + (rand() % 100 / 100.0f);

        this->particlesContainer[particleIdx].life = initialLife;
        this->particlesContainer[particleIdx].maxLife = initialLife;

        const float offsetX = ((rand() % 1000 / 1000.0f) - 0.5f) * params.range;
        const float offsetZ = ((rand() % 1000 / 1000.0f) - 0.5f) * params.range;

        this->particlesContainer[particleIdx].pos = glm::vec3(
            params.spawnPoint.x + offsetX,
            params.spawnPoint.y,
            params.spawnPoint.z + offsetZ);

        const float vx = ((rand() % 100 - 50) / 250.0f);
        const float vz = ((rand() % 100 - 50) / 250.0f);
        const float vy = 1.0f + (rand() % 100 / 100.0f);
        this->particlesContainer[particleIdx].speed = glm::vec3(vx, vy, vz);

        this->particlesContainer[particleIdx].size = params.initialSize;

        this->particlesContainer[particleIdx].color = glm::vec4(params.color1, 1);
    }

    for (int i = 0; i < MAX_PARTICLES; i++)
    {
        Particle &p = this->particlesContainer[i];
        if (p.life > 0.0)
        {
            p.life -= delta;
            p.pos += p.speed * static_cast<float>(delta);

            const float lifeRatio = p.life / p.maxLife;

            glm::vec3 finalRGB;
            if (lifeRatio > 0.5f)
            {
                finalRGB = glm::mix(params.color2, params.color1, (lifeRatio - 0.5f) * 2.0f);
            }
            else
            {
                finalRGB = glm::mix(params.color3, params.color2, lifeRatio * 2.0f);
            }

            p.size = params.initialSize * lifeRatio;

            const float alpha = lifeRatio;

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

ParticleRenderer::~ParticleRenderer()
{
    delete[] this->g_particule_color_data;
    delete[] this->g_particule_position_size_data;
}
