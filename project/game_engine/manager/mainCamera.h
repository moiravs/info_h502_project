
#ifndef INFOH502_CPP_MAINCAMERA_H
#define INFOH502_CPP_MAINCAMERA_H
#include "../entity/camera.h"

class MainCamera
{
    MainCamera() = default;
    static std::shared_ptr<Camera> instance;
public:
    static std::shared_ptr<Camera> get();
};

#endif //INFOH502_CPP_MAINCAMERA_H