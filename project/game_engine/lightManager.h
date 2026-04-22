
#ifndef INFOH502_CPP_LIGHTMANAGER_H
#define INFOH502_CPP_LIGHTMANAGER_H
#include <memory>

#include "shader.h"

struct GPULight
{
    glm::vec3 position;
    float pad1;

    float ambient_strength;
    float diffuse_strength;
    float specular_strength;
    float constant;

    float linear;
    float quadratic;
    float pad2;
    float pad3;
};

class LightManager {
    static std::shared_ptr<LightManager> instance;
    std::vector<std::shared_ptr<Light>> lights {};
    std::vector<std::shared_ptr<Shader>> shaders {};
    GLuint ubo{};
    LightManager();
    void rewriteBuffer() const;
public:
    static std::shared_ptr<LightManager> get();
    void notify() const;
    void registerLight(std::shared_ptr<Light> light);
    void registerShader(std::shared_ptr<Shader> shader);
    ~LightManager();
};

#endif //INFOH502_CPP_LIGHTMANAGER_H
