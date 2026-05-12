
#ifndef INFOH502_CPP_PROPBUILDER_H
#define INFOH502_CPP_PROPBUILDER_H

#include <vector>

#include "../renderable.h"
#include "../entity/object.h"

class Prop : public Renderable
{
    std::vector<std::shared_ptr<Renderable>> _renderables{};
    std::shared_ptr<Object> _mainObject = nullptr;

public:
    Prop() = default;
    void addRenderable(const std::shared_ptr<Renderable> &renderable);
    void setMainObject(const std::shared_ptr<Object> &mainObject);
    void update(float delta) override;
    [[nodiscard]] std::shared_ptr<Object> getMainObject();
    void setPosition(const glm::vec3 &position) const;
    void render() override;
    std::vector<std::shared_ptr<Renderable>> getRenderables();
    void renderWithShader(const std::shared_ptr<Shader> &shader) override;
    void removeRenderable(std::shared_ptr<Renderable> indexToRemove);
};

#endif // INFOH502_CPP_PROPBUILDER_H
