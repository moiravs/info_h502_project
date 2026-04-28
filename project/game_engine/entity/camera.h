#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "controllable.h"
#include "entity.h"
#include "../../utils/constants.h"
#include "../manager/uboProvider.h"

struct CameraInfo
{
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec4 cameraPos;
    glm::vec4 cameraRight;
    glm::vec4 cameraUp;
};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera : public Entity, public UboProvider, public Controllable
{
    // camera Attributes
    glm::vec3 flatFront{};
    // camera options
    float zoom;

protected:
    void updateRotation() override;

public:
    // constructor with vectors
    explicit Camera(glm::vec3 up = WORLD_UP,
                    float yaw = YAW, float pitch = PITCH, float roll = ROLL);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    [[nodiscard]] glm::mat4 getViewMatrix() const;

    static glm::mat4 getProjectionMatrix(float fov = 45.0, float ratio = 1.0, float near = 0.01, float far = 100.0);

    [[nodiscard]] float getZoom() const;

    void invertPitch();

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboardMovement(MovementDirection direction, float deltaTime) override;

    void updateUBO() const;

    void prepareReflection(int height);

    void resetCameraAfterReflection(int height);

    void processKeyboardRotation(float yawRot, float pitchRot, float rollRot, float deltaTime) override;

    void setLookAt(glm::vec3 target);

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void processMouseMovement(float xoffset, float yoffset, float zoffset, float deltaTime) override;

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void processMouseScroll(float yoffset);
};

#endif