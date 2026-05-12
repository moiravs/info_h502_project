#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../utils/constants.h"
#include "../culling/frustum.h"
#include "../depth/depthCam.h"
#include "../manager/uboProvider.h"
#include "controllable.h"
#include "entity.h"

struct CameraInfo
{
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec4 cameraPos;
    glm::vec4 cameraRight;
    glm::vec4 cameraUp;
};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera : public Entity, public UboProvider, public Controllable, public DepthCam
{
    // camera Attributes
    glm::vec3 flatFront{};
    // camera options
    float zoom;
    std::shared_ptr<Frustum> _frustum = nullptr;
    std::shared_ptr<Entity> lockedEntity = nullptr;
    bool lookingAt = true;
    int _width;
    int _height;

protected:
    void updateRotation() override;

    void updateFrustum() const;

    void updateLook();

    bool shouldClampPitch() const override;

public:
    // constructor with vectors
    explicit Camera(int src_width, int src_height, glm::vec3 up = WORLD_UP,
                    float yaw = YAW, float pitch = PITCH, float roll = ROLL);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    [[nodiscard]] glm::mat4 getViewMatrix() const;

    glm::mat4 getProjectionMatrix() const;

    [[nodiscard]] float getZoom() const;
    [[nodiscard]] float getFOV() const;
    [[nodiscard]] float getAspectRatio() const;

    glm::mat4 getPV() const override;

    void invertPitch();

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void processKeyboardMovement(MovementDirection direction, double deltaTime) override;

    void updateUBO() const;

    void prepareReflection(int height);

    using Entity::setPosition;
    void setPosition(const glm::vec3 &position) override;

    void resetCameraAfterReflection(int height);

    void processScroll(double dx, double dy) override;

    void processRotation(double yawRot, double pitchRot, double rollRot, double deltaTime) override;

    void lookAt(const glm::vec3 &target);
    void forceLookAt(const std::shared_ptr<Entity> &entity);

    bool canView(const std::array<glm::vec3, 8> &bounds) const;

    std::shared_ptr<Frustum> getFrustum() const;

    std::vector<glm::vec3> getFrustumCorners() const;
};

#endif