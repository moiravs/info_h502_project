
#ifndef Renderer_H
#define Renderer_H
#include "../shader.h"
#include "glad/glad.h"

class RenderableEntity;

class Renderer
{
protected:
    std::shared_ptr<Shader> _shader = nullptr;
    std::shared_ptr<RenderableEntity> _entity = nullptr;
    std::vector<GLuint> _VAOs {}, _VBOs {};

    [[nodiscard]] static std::shared_ptr<Shader> generateShader(const std::string& shaderName);

    void createVAOs(size_t nb);
    void createVBOs(size_t nb);
    void clearVAOs() const;
    void clearVBOs() const;

    virtual void setupVAOs() = 0;
public:
    explicit Renderer(std::shared_ptr<Shader> shader);
    virtual void updateUniforms() const = 0;
    virtual ~Renderer();
    virtual void registerEntity(const std::shared_ptr<RenderableEntity> &entity);
    // this method should eventually be protected/removed
    std::shared_ptr<Shader> getShader();
    virtual void render();
};

#endif