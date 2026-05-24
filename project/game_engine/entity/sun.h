
#ifndef INFOH502_CPP_SUN_H
#define INFOH502_CPP_SUN_H
#include "object.h"


class Sun : public Object {
    float orbitSpeed = 0.0f;
    float orbitRadius = 0.0f;
public:
    explicit Sun(const std::shared_ptr<Mesh>& mesh, float speed, float orbitRadius, const std::shared_ptr<Renderer>& renderer);
    void update(float delta) override;
};



#endif //INFOH502_CPP_SUN_H
