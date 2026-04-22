
#ifndef INFOH502_CPP_MAINCAMERA_H
#define INFOH502_CPP_MAINCAMERA_H
#include <memory>

#include "entity/camera.h"

class MainCamera
{
    static std::shared_ptr<Camera> instance;
    MainCamera() = default;

public:
    static std::shared_ptr<Camera> get();
};

#endif //INFOH502_CPP_MAINCAMERA_H
