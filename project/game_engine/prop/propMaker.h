
#ifndef INFOH502_CPP_PROPMAKER_H
#define INFOH502_CPP_PROPMAKER_H
#include <memory>

#include "prop.h"


class PropMaker {
    PropMaker();
public:
    static std::shared_ptr<Prop> makeLamp(const glm::vec3& position, float radius, const glm::vec3& color,
        const glm::vec4& lightProperties, const glm::vec3& lightAttenuation);
};



#endif //INFOH502_CPP_PROPMAKER_H
