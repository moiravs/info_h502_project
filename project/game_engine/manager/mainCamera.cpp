#include "mainCamera.h"

std::shared_ptr<Camera> MainCamera::instance = nullptr;

void MainCamera::init(int width, int height)
{
    if (instance == nullptr)
    {
        instance = std::make_shared<Camera>(width, height);
    }
}

std::shared_ptr<Camera> MainCamera::get()
{
    return instance; // Note: Might return nullptr if init() wasn't called!
}