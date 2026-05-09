
#ifndef INFOH502_CPP_DIRECTIONALLIGHT_H
#define INFOH502_CPP_DIRECTIONALLIGHT_H

#include "../../depth/depthCam.h"
#include "light.h"


class Frustum;
class DirectionalLight : public Light, public DepthCam {
    glm::vec3 _target;
    DirectionalLight(float x, float y, float z);
public:
    static std::shared_ptr<DirectionalLight> make(float x = 0, float y = 0, float z = 0);

    void setTarget(const glm::vec3 &target);

    glm::mat4 getPV() const override;

    [[nodiscard]] glm::vec3 getTarget() const;
    glm::vec3 getDirection() const;
};



#endif //INFOH502_CPP_DIRECTIONALLIGHT_H
