#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum PlayerMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    CROUCH,
    JUMP
};

// Default camera values
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float MOV_SPEED = 100.0f;
constexpr float ROT_SPEED = 100.0f;
constexpr float SENSITIVITY = 0.2f;
constexpr float ZOOM = 45.0f;
constexpr auto WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 FlatFront;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float RotationSpeed;
    float MouseSensitivity;
    float Zoom;

    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
public:

    // constructor with vectors
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = WORLD_UP,
                    float yaw = YAW, float pitch = PITCH);

    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    [[nodiscard]] glm::mat4 getViewMatrix() const;

    static glm::mat4 getProjectionMatrix(float fov = 45.0, float ratio = 1.0, float near = 0.01, float far = 100.0);

    [[nodiscard]] glm::vec3 getUp() const;

    [[nodiscard]] float getZoom() const;

    [[nodiscard]] glm::vec3 getPosition() const;

    void invertPitch();

    [[nodiscard]] glm::vec3 getCameraRight() const;

    [[nodiscard]] glm::vec3 getCameraPosition() const;

    void setCameraPosition(glm::vec3 position);

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboardMovement(PlayerMovement direction, float deltaTime);

    void prepareReflection(int height);

    void resetCameraAfterReflection(int height);

    void processKeyboardRotation(float YawRot, float PitchRot, float deltaTime, GLboolean constrainPitch = true);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset, float deltaTime, GLboolean constrainPitch = true);

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(const float yoffset);
};

#endif