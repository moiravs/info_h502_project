#include "mainCamera.h"

std::shared_ptr<Camera> MainCamera::instance = nullptr;

std::shared_ptr<Camera> MainCamera::get()
{
    if (instance == nullptr)
        instance = std::make_shared<Camera>(glm::vec3(0.0, 0.0, 0.1));

    return instance;
}
