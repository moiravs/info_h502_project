
#ifndef INFOH502_CPP_PROPMAKER_H
#define INFOH502_CPP_PROPMAKER_H
#include <memory>

#include "prop.h"
#include "../mesh/terrainMesh.h"

class PropMaker
{
    PropMaker() = default;

public:
    static std::shared_ptr<Prop> makeLamp(const glm::vec3 &position, const glm::vec3 &scale, const glm::vec3 &color,
                                          const glm::vec4 &lightProperties, const glm::vec3 &lightAttenuation);

    static std::shared_ptr<Prop> makeTrees(const std::shared_ptr<TerrainMesh> &heightMap);

    static std::shared_ptr<Prop> makeFirecamp(const std::shared_ptr<TerrainMesh> &heightMap);

    static std::shared_ptr<Prop> makePlane(const std::shared_ptr<TerrainMesh> &heightMap);
};

#endif // INFOH502_CPP_PROPMAKER_H
