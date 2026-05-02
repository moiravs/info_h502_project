
#ifndef INFOH502_CPP_PROPMAKER_H
#define INFOH502_CPP_PROPMAKER_H
#include <memory>

#include "prop.h"

class HeightMap;

class PropMaker
{
    PropMaker() = default;

public:
    static std::shared_ptr<Prop> makeLamp(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &color,
                                          const glm::vec4 &lightProperties, const glm::vec3 &lightAttenuation);

    static std::shared_ptr<Prop> makeTrees(const std::shared_ptr<HeightMap> &heightMap);

    static std::shared_ptr<Prop> makeFirecamp(float x, float z, const std::shared_ptr<HeightMap> &heightMap);

    static std::shared_ptr<Prop> makePlane();
};

#endif // INFOH502_CPP_PROPMAKER_H
