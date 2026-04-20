
#ifndef INFOH502_CPP_MAINCAMERA_H
#define INFOH502_CPP_MAINCAMERA_H
#include <memory>

#include "entity/camera.h"

class MainCamera
{
    static std::shared_ptr<Camera> instance;
    MainCamera() = default;

public:
    static std::shared_ptr<Camera> get()
    {
        if (instance == nullptr)
            instance = std::make_shared<Camera>(glm::vec3(0.0, 0.0, 0.1));

        return instance;
    }
};

#endif //INFOH502_CPP_MAINCAMERA_H
