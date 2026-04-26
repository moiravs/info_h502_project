
#ifndef INFOH502_CPP_PARTICLEGENERATOR_H
#define INFOH502_CPP_PARTICLEGENERATOR_H

#include "renderableEntity.h"
class ParticleRenderer;

struct ParticleParams
{
    float spread;
    float range;
    float initialSize;
    float maxLife = 1.f;
    glm::vec3 color1;
    glm::vec3 color2;
    glm::vec3 color3;
};

class ParticleGenerator: public RenderableEntity {
    ParticleParams _params;
public:
    explicit ParticleGenerator(const ParticleParams& params, const std::shared_ptr<ParticleRenderer>& renderer);
    static std::shared_ptr<ParticleGenerator> make(const ParticleParams& params);
    void update(float delta) override;
    std::shared_ptr<ParticleRenderer> getParticleRenderer() const;
    bool shouldUpdate() const override;
    const ParticleParams& getParams() const;
};

#endif //INFOH502_CPP_PARTICLEGENERATOR_H
