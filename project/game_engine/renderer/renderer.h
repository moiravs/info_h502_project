
#ifndef Renderer_H
#define Renderer_H
#include "../shader.h"
#include "glad/glad.h"
#include "../../utils/utils.h"

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

    template <typename T>
    std::shared_ptr<T> getEntity() const
    {
        const auto obj = std::dynamic_pointer_cast<T>(this->_entity);
        if (!obj)
        {
            ERROR("The entity linked to this renderer is not of the correct type");
        }
        return obj;
    }
public:
    explicit Renderer(std::shared_ptr<Shader> shader);
    virtual void updateUniforms() const = 0;
    virtual ~Renderer();
    virtual void registerEntity(const std::shared_ptr<RenderableEntity> &entity);
    virtual void render();
};

#endif