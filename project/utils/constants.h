#ifndef INFOH502_CPP_CONSTANTS_H
#define INFOH502_CPP_CONSTANTS_H

#include "glm/glm.hpp"

constexpr auto WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

constexpr int REFLECTION_WIDTH = 320;
constexpr int REFLECTION_HEIGHT = 180;
constexpr int REFRACTION_WIDTH = 1280;
constexpr int REFRACTION_HEIGHT = 720;

// Default camera values
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float MOV_SPEED = 100.0f;
constexpr float ROT_SPEED = 100.0f;
constexpr float SENSITIVITY = 0.2f;
constexpr float ZOOM = 45.0f;

constexpr int MAX_LIGHTS = 128;

// Bindings
const std::vector<std::string> UBO_BINDING_NAMES = {
    "WaterData", "Lights", "CameraInfo"
};

#endif //INFOH502_CPP_CONSTANTS_H
