#ifndef INFOH502_CPP_CONSTANTS_H
#define INFOH502_CPP_CONSTANTS_H

#include "glm/glm.hpp"
#include <string>

#include "glad/glad.h"

constexpr auto WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

constexpr bool DEFAULT_CAMERA_LOCKED_ON_PLAYER = true;

constexpr unsigned int SCR_WIDTH = 1920;
constexpr unsigned int SCR_HEIGHT = 1080;
constexpr float ASPECT_RATIO = static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT);

constexpr unsigned int DEPTH_WIDTH = 4096;
constexpr unsigned int DEPTH_HEIGHT = 4096;

constexpr int PLAN_SIZE_X = 1000;
constexpr int WATER_HEIGHT = 0;

constexpr int REFLECTION_WIDTH = SCR_WIDTH / 2;
constexpr int REFLECTION_HEIGHT = SCR_HEIGHT / 2;
constexpr int REFRACTION_WIDTH = SCR_WIDTH / 2;
constexpr int REFRACTION_HEIGHT = SCR_HEIGHT / 2;

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

// plane
constexpr float PLANE_PITCH_SPEED = 2.f;
constexpr float PLANE_ROLL_SPEED = 2.f;
constexpr float PLANE_TURN_SPEED = 1.f;
constexpr float PLANE_SPEED = 0.4f;
constexpr float PLANE_ROLL_AMORTIZE = 0.01f;
constexpr float PLANE_SPINNER_RAD_PER_SEC = 30.f;

constexpr int MAX_LIGHTS = 128;
constexpr int CHUNK_SIZE = 128;

constexpr unsigned MAX_PARTICLES = 10000;

// SHADERS

constexpr int COLOR_TEX = GL_TEXTURE0;
constexpr int COLOR_TEX_IDX = 0;
constexpr int NORMAL_TEX = GL_TEXTURE1;
constexpr int NORMAL_TEX_IDX = 1;
constexpr int DEPTH_TEX = GL_TEXTURE2;
constexpr int DEPTH_TEX_IDX = 2;
constexpr int MATERIAL_TEX = GL_TEXTURE3;
constexpr int MATERIAL_TEX_IDX = 3;
constexpr int LIT_COLOR_TEX = GL_TEXTURE4;
constexpr int LIT_COLOR_TEX_IDX = 4;
constexpr int REFLECTION_TEX = GL_TEXTURE5;
constexpr int REFLECTION_TEX_IDX = 5;
constexpr int REFRACTION_TEX = GL_TEXTURE6;
constexpr int REFRACTION_TEX_IDX = 6;
constexpr int DUDV_TEX = GL_TEXTURE7;
constexpr int DUDV_TEX_IDX = 7;
constexpr int SHADOW_TEX = GL_TEXTURE8;
constexpr int SHADOW_TEX_IDX = 8;
constexpr int GRASS_TEX = GL_TEXTURE10;
constexpr int GRASS_TEX_IDX = 10;
constexpr int ROCK_TEX = GL_TEXTURE11;
constexpr int ROCK_TEX_IDX = 11;
constexpr int SNOW_TEX = GL_TEXTURE12;
constexpr int SNOW_TEX_IDX = 12;

// Bindings
const std::vector<std::string> UBO_BINDING_NAMES = {
    "WaterData", "Lights", "CameraInfo", "Mist"};

#endif // INFOH502_CPP_CONSTANTS_H
