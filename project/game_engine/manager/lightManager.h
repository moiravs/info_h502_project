
#ifndef INFOH502_CPP_LIGHTMANAGER_H
#define INFOH502_CPP_LIGHTMANAGER_H

#include <memory>

#include "uboProvider.h"
#include "../shader.h"

struct LightBlock
{
    glm::vec4 positions[MAX_LIGHTS];    // x=x,        y=y,       z=z,         w=UNUSED
    glm::vec4 properties[MAX_LIGHTS];   // x=ambiant,  y=diffuse, z=specular,  w=shininess
    glm::vec4 attenuations[MAX_LIGHTS]; // x=constant, y=linear,  z=quadratic, w=UNUSED
    glm::vec4 colors[MAX_LIGHTS];       // x=r,        y=g,       z=b,         w=UNUSED
    int count;
    int pad1;
    int pad2;
    int pad3;
};

class LightManager : public UboProvider {
    std::vector<std::shared_ptr<Light>> lights {};
    bool needsUpdate = false;
    LightManager();
public:
    static LightManager& get();
    void notify();
    void updateUBO();
    void registerLight(std::shared_ptr<Light> light);
};

#endif //INFOH502_CPP_LIGHTMANAGER_H
