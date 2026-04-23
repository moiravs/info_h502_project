
#ifndef INFOH502_CPP_LIGHTMANAGER_H
#define INFOH502_CPP_LIGHTMANAGER_H

#include <memory>

#include "../shader.h"

struct LightBlock
{
    glm::vec4 positions[MAX_LIGHTS];    // x=x,        y=y,       z=z,         w=UNUSED
    glm::vec4 properties[MAX_LIGHTS];   // x=ambiant,  y=diffuse, z=specular,  w=shininess
    glm::vec4 attenuations[MAX_LIGHTS]; // x=constant, y=linear,  z=quadratic, w=UNUSED
    int count;
    int pad1;
    int pad2;
    int pad3;
};

class LightManager {
    std::vector<std::shared_ptr<Light>> lights {};
    GLuint ubo{};
    bool needsUpdate = false;
    LightManager();
    void rewriteBuffer() const;
public:
    static LightManager& get();
    void notify();
    void applyChanges();
    void registerLight(std::shared_ptr<Light> light);
    ~LightManager();
};

#endif //INFOH502_CPP_LIGHTMANAGER_H
