
#ifndef INFOH502_CPP_PROPMAKER_H
#define INFOH502_CPP_PROPMAKER_H
#include <memory>

#include "prop.h"


class DirectionalLight;
class HeightMap;

class PropMaker
{
    PropMaker() = default;

public:
    static std::shared_ptr<Prop> makeLamp(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &color,
                                          const glm::vec4 &lightProperties, const glm::vec3 &lightAttenuation);

    static std::shared_ptr<Prop> makeTrees(const std::shared_ptr<HeightMap> &heightMap, int nbTrees = 200, int chunkSize = 256);
    static std::shared_ptr<Prop> makeFlowers(const std::shared_ptr<HeightMap> &heightMap, int nbTrees = 300, int chunkSize = 256);
    static std::shared_ptr<Prop> makeRings(const std::shared_ptr<HeightMap> &heightMap);

    static std::shared_ptr<Prop> makeFirecamp(float x, float z, const std::shared_ptr<HeightMap> &heightMap);

    static std::pair<std::shared_ptr<Prop>, std::shared_ptr<DirectionalLight>> makeSun(const glm::vec3 &position,
        const glm::vec3 &scale, const glm::vec3 &color);

    static std::shared_ptr<Prop> makePlane(const std::shared_ptr<HeightMap>& heightMap);
};

#endif // INFOH502_CPP_PROPMAKER_H
