
#ifndef INFOH502_CPP_PROPBUILDER_H
#define INFOH502_CPP_PROPBUILDER_H

#include <vector>

#include "../renderable.h"
#include "../renderer/renderer.h"

class Prop : public Renderable {
    std::vector<std::shared_ptr<Renderer>> _renderers{};
    std::vector<std::shared_ptr<Entity>> _entities{};
    std::shared_ptr<Object> _mainObject = nullptr;
public:
    Prop()=default;
    void addRenderer(const std::shared_ptr<Renderer>& renderer);
    void addEntity(const std::shared_ptr<Entity>& entity);
    void setMainObject(const std::shared_ptr<Object>& mainObject);
    [[nodiscard]] std::shared_ptr<Object> getMainObject();
    void setPosition(const glm::vec3& position) const;
    void render() override;
};

#endif //INFOH502_CPP_PROPBUILDER_H
