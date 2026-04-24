#include "mainCamera.h"

std::shared_ptr<Camera> MainCamera::instance = nullptr;

std::shared_ptr<Camera> MainCamera::get()
{
    if (instance == nullptr)
        instance = std::make_shared<Camera>();

    return instance;
}
