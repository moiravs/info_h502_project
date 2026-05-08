#include "player.h"

#include <iostream>

#include "../../utils/utils.h"
#include "../manager/mainCamera.h"
#include "../mesh/heightMap.h"
#include "glm/ext/scalar_common.hpp"
#include "glm/gtc/type_ptr.hpp"

Player::Player(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<HeightMap>& heightMap, const std::shared_ptr<Renderer>& renderer)
: Object(mesh, renderer), _heightMap(heightMap) {}

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

    const auto offset = this->findCameraOffset();
    this->getCameraAttachment()->offset = offset;
    this->updatePositionAttached(this->getCameraAttachment());
}

glm::vec3 Player::findCameraOffset() const
{
    const glm::quat qYaw   = glm::angleAxis(-this->_yawCam,   glm::vec3(0, 1, 0));
    const glm::quat qPitch = glm::angleAxis(-this->_pitchCam, glm::vec3(1, 0, 0));

    const glm::vec3 direction = glm::normalize(qYaw * qPitch) * glm::vec3(0, 0, 1);
    float maxDist = this->_camDistance;
    float minDist = MIN_CAM_DIST;
    auto currentOffset = maxDist * direction;
    auto currentPosition = currentOffset + this->getPosition();

    auto isBelowTerrain = [this](const glm::vec3 &position) -> bool
    {
        return this->_heightMap->getHeight(position.x, position.z) >= position.y - 0.5f;
    };

    if (!isBelowTerrain(currentPosition)) return currentOffset;

    glm::vec3 bestOffset = direction * minDist;

    while (abs(maxDist - minDist) > 0.5f)
    {
        const float avgDist = (maxDist + minDist) * 0.5f;

        currentOffset = direction * avgDist;
        currentPosition = currentOffset + this->getPosition();

        if (isBelowTerrain(currentPosition))
        {
            maxDist = avgDist;
        }
        else
        {
            minDist = avgDist;
            bestOffset = {currentOffset.x, currentOffset.y, currentOffset.z};
        }
    }

    return bestOffset;
}

void Player::update(const float delta)
{
    this->rotate(0, -this->getPitch() * PLANE_ROLL_AMORTIZE, -this->getRoll() * PLANE_ROLL_AMORTIZE);
    this->rotate(this->getRoll() * PLANE_TURN_SPEED * delta, 0, 0);
    this->setPosition(this->getPosition() + this->getFront() * PLANE_SPEED);

    RenderableEntity::update(delta);
}

bool Player::shouldUpdate() const { return true; }
