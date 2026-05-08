#ifndef PLAYER_H
#define PLAYER_H

#include "camera.h"
#include "object.h"

class HeightMap;

class Player : public Object, public Controllable
{
    float _camDistance = DEFAULT_CAMERA_DISTANCE_TO_PLAYER;
    float _yawCam = 0;
    float _pitchCam = 0;

    std::shared_ptr<HeightMap> _heightMap = nullptr;

    std::shared_ptr<Attachment> getCameraAttachment();

    void updateCameraOffset();

    glm::vec3 findCameraOffset() const;

public:
    explicit Player(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<HeightMap>& heightMap, const std::shared_ptr<Renderer> &renderer);

    void update(float delta) override;

    bool shouldUpdate() const override;

    void attachCamera();

    void processKeyboardMovement(MovementDirection direction, double deltaTime) override;

    void processRotation(double yawRot, double pitchRot, double rollRot, double deltaTime) override;

    void processScroll(double dx, double dy) override;

    void processScrollAlt(double dx, double dy) override;

    void processRotationAlt(double yawRot, double pitchRot, double rollRot, double deltaTime) override;
};

#endif