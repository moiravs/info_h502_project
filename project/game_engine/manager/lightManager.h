
#ifndef INFOH502_CPP_LIGHTMANAGER_H
#define INFOH502_CPP_LIGHTMANAGER_H

#include <memory>

#include "../shader/shader.h"
#include "uboProvider.h"


class DirectionalLight;
class PointLight;

struct LightBlock
{
    glm::vec4 positions[MAX_LIGHTS];    // x=x,        y=y,       z=z,         w=UNUSED
    glm::vec4 properties[MAX_LIGHTS];   // x=ambiant,  y=diffuse, z=specular,  w=shininess
    glm::vec4 attenuations[MAX_LIGHTS]; // x=constant, y=linear,  z=quadratic, w=UNUSED
    glm::vec4 colors[MAX_LIGHTS];       // x=r,        y=g,       z=b,         w=UNUSED
    glm::vec4 sunPos;
    int count;
    int pad1;
    int pad2;
    int pad3;
};

class LightManager : public UboProvider {
    std::vector<std::shared_ptr<PointLight>> lights {};
    std::shared_ptr<DirectionalLight> _sun;
    bool needsUpdate = false;
    LightManager();
public:
    static LightManager& get();
    void notify();
    void updateUBO();
    void registerPointLight(std::shared_ptr<PointLight> light);
    void registerDirectionalLight(const std::shared_ptr<DirectionalLight> &light);
};

#endif //INFOH502_CPP_LIGHTMANAGER_H
