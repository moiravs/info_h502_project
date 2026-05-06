#include "player.h"

#include <iostream>

#include "../../utils/utils.h"
#include "../manager/mainCamera.h"
#include "glm/ext/scalar_common.hpp"
#include "glm/gtc/type_ptr.hpp"

Player::Player(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Renderer>& renderer): Object(mesh, renderer){}

void Player::processKeyboardMovement(const MovementDirection direction, const double deltaTime)
{
    if (direction == LEFT)
        this->rotate(0, 0, -PLANE_ROLL_SPEED * deltaTime);
    if (direction == RIGHT)
        this->rotate(0, 0, PLANE_ROLL_SPEED * deltaTime);
    if (direction == BACKWARD)
        this->rotate(0, PLANE_PITCH_SPEED * deltaTime, 0);
    if (direction == FORWARD)
        this->rotate(0, -PLANE_PITCH_SPEED * deltaTime, 0);
}

void Player::processRotation(const double yawRot, const double pitchRot, const double rollRot, const double deltaTime)
{
    const double velocity = ROT_SPEED * deltaTime;

    this->_yawCam += velocity * yawRot;
    this->_pitchCam += velocity * pitchRot;

    this->updateCameraOffset();
}

void Player::processScroll(const double dx, const double dy)
{
    this->_camDistance += dy;
    this->_camDistance = glm::clamp(this->_camDistance, MIN_CAM_DIST, MAX_CAM_DIST);

    this->updateCameraOffset();
}

void Player::processScrollAlt(const double dx, const double dy)
{
    MainCamera::get()->processScroll(dx, dy);
}

void Player::processRotationAlt(double yawRot, double pitchRot, double rollRot, double deltaTime)
{

}

void Player::attachCamera()
{
    this->attach(MainCamera::get());
    this->updateCameraOffset();
}

std::shared_ptr<Entity::Attachment> Player::getCameraAttachment()
{
    return this->getAttachment(MainCamera::get());
}

void Player::updateCameraOffset()
{
    if (this->_yawCam > glm::pi<float>())
        this->_yawCam -= 2 * glm::pi<float>();
    if (this->_yawCam < -glm::pi<float>())
        this->_yawCam += 2 * glm::pi<float>();
    this->_pitchCam = glm::clamp(this->_pitchCam, -1.57f, 1.57f);

    const glm::quat qYaw   = glm::angleAxis(-this->_yawCam,   glm::vec3(0, 1, 0));
    const glm::quat qPitch = glm::angleAxis(-this->_pitchCam, glm::vec3(1, 0, 0));

    const glm::quat trans = glm::normalize(qYaw * qPitch);
    const auto offset = glm::vec3(this->_camDistance) * (trans * glm::vec3(0, 0, 1));
    this->getCameraAttachment()->offset = offset;
    this->updatePositionAttached(this->getCameraAttachment());
}

void Player::update(const float delta)
{
    this->rotate(0, -this->getPitch() * PLANE_ROLL_AMORTIZE, -this->getRoll() * PLANE_ROLL_AMORTIZE);
    this->rotate(this->getRoll() * PLANE_TURN_SPEED * delta, 0, 0);
    this->setPosition(this->getPosition() + this->getFront() * PLANE_SPEED);

    RenderableEntity::update(delta);
}

bool Player::shouldUpdate() const { return true; }
