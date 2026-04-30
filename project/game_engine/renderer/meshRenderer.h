
#ifndef INFOH502_CPP_MESHRENDERER_H
#define INFOH502_CPP_MESHRENDERER_H
#include "renderer.h"
#include <glm/gtx/string_cast.hpp>

class Object;

class MeshRenderer : public Renderer
{

protected:
    GLuint emptyTexture;
    unsigned int shadowMapTexture;
    glm::mat4 lightSpaceMatrix;
    void setupVAOs() override;

public:
    explicit MeshRenderer(const std::string &shaderName);
    explicit MeshRenderer(const std::shared_ptr<Shader> shaderName);
    void updateUniforms() const override;
    void registerEntity(const std::shared_ptr<RenderableEntity> &entity) override;
    void setShadowData(unsigned int textureID, const glm::mat4 &matrix)
    {
        this->shadowMapTexture = textureID;
        this->lightSpaceMatrix = matrix;
        std::cout << "texId " << textureID << " matrix " << glm::to_string(matrix) << std::endl;
    }

    virtual void drawElements(int numTriangles);
    void renderShadows(const std::shared_ptr<Shader> &shadowShader);

    void render() override;
};

#endif // INFOH502_CPP_MESHRENDERER_H
