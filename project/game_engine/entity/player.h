#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "camera.h"

class Player : public Object, public Controllable
{
    float _camDistance = DEFAULT_CAMERA_DISTANCE_TO_PLAYER;
    float _yawCam = 0;
    float _pitchCam = 0;

    std::shared_ptr<Attachment> getCameraAttachment();

    void updateCameraOffset();

public:
    explicit Player(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Renderer> &renderer);

    void attachCamera();

    void processKeyboardMovement(MovementDirection direction, double deltaTime) override;

    void processRotation(double yawRot, double pitchRot, double rollRot, double deltaTime) override;

    void processScroll(double dx, double dy) override;

    void processScrollAlt(double dx, double dy) override;

    void processRotationAlt(double yawRot, double pitchRot, double rollRot, double deltaTime) override;
};

#endif