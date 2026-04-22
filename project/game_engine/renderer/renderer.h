
#ifndef Renderer_H
#define Renderer_H
#include "../entity/object.h"
#include "../shader.h"
#include "glad/glad.h"

class Object;

class Renderer
{
protected:
    std::shared_ptr<Object> _object = nullptr;
    std::shared_ptr<Shader> _shader = nullptr;
    GLuint VAO = 0, VBO = 0;

    [[nodiscard]] std::shared_ptr<Shader> generateShader() const;
    [[nodiscard]] virtual std::string getShaderName() const = 0;
public:
    explicit Renderer(std::shared_ptr<Shader> shader);
    virtual void updateUniforms() const = 0;
    virtual ~Renderer();
    virtual void registerObject(std::shared_ptr<Object> object);
    // this method should eventually be protected/removed
    std::shared_ptr<Shader> getShader();
    virtual void render();
};

#endif