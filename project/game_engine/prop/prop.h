
#ifndef INFOH502_CPP_PROPBUILDER_H
#define INFOH502_CPP_PROPBUILDER_H

#include <vector>

#include "../renderable.h"
#include "../entity/object.h"

class Prop : public Renderable {
    std::vector<std::shared_ptr<Renderable>> _renderables{};
    std::shared_ptr<Object> _mainObject = nullptr;
public:
    Prop()=default;
    void addRenderable(const std::shared_ptr<Renderable>& renderable);
    void setMainObject(const std::shared_ptr<Object>& mainObject);
    [[nodiscard]] std::shared_ptr<Object> getMainObject();
    void setPosition(const glm::vec3& position) const;
    void render(float delta) override;
    void renderDepth(const std::shared_ptr<DepthMap>& depthMap) override;
};

#endif //INFOH502_CPP_PROPBUILDER_H
