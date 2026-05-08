#ifndef INFOH502_CPP_CONSTANTS_H
#define INFOH502_CPP_CONSTANTS_H

#include "glm/glm.hpp"
#include <string>

constexpr auto WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

constexpr bool DEFAULT_CAMERA_LOCKED_ON_PLAYER = true;

constexpr unsigned int SCR_WIDTH = 800;
constexpr unsigned int SCR_HEIGHT = 600;

constexpr unsigned int DEPTH_WIDTH = 1024;
constexpr unsigned int DEPTH_HEIGHT = 1024;

constexpr int PLAN_SIZE_X = 1000;
constexpr int WATER_HEIGHT = 0;

constexpr int REFLECTION_WIDTH = 320;
constexpr int REFLECTION_HEIGHT = 180;
constexpr int REFRACTION_WIDTH = 1280;
constexpr int REFRACTION_HEIGHT = 720;

// Default camera values
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float ROLL = 0.0f;
constexpr float MOV_SPEED = 100.0f;
constexpr float ROT_SPEED = 2.5f;
constexpr float SENSITIVITY = .08f;
constexpr float ZOOM = 45.0f;
constexpr float FAR = 1000.f;
constexpr float NEAR = 0.01f;
constexpr float FOVY = 45.f;
constexpr float MIN_ZOOM = 0.1f;
constexpr float MAX_ZOOM = 120.f;
constexpr float DEFAULT_CAMERA_DISTANCE_TO_PLAYER = 30.f;
constexpr float MIN_CAM_DIST = 5.f;
constexpr float MAX_CAM_DIST = 100.f;
constexpr float PLANE_PITCH_SPEED = 2.f;
constexpr float PLANE_ROLL_SPEED = 2.f;
constexpr float PLANE_TURN_SPEED = 1.f;
constexpr float PLANE_SPEED = 0.4f;
constexpr float PLANE_ROLL_AMORTIZE = 0.01f;

constexpr int MAX_LIGHTS = 128;
constexpr int CHUNK_SIZE = 128;

constexpr unsigned MAX_PARTICLES = 10000;

// Bindings
const std::vector<std::string> UBO_BINDING_NAMES = {
    "WaterData", "Lights", "CameraInfo", "Mist"};

#endif // INFOH502_CPP_CONSTANTS_H
