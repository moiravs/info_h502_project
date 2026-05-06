#include "player.h"

#include <iostream>

#include "../../utils/utils.h"
#include "../manager/mainCamera.h"
#include "glm/gtc/type_ptr.hpp"

Player::Player(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Renderer>& renderer): Object(mesh, renderer){}

void Player::processKeyboardMovement(const MovementDirection direction, const double deltaTime)
{
    const float velocity = MOV_SPEED * deltaTime;
    if (direction == LEFT)
        this->setPosition(this->getPosition() - this->getRight() * velocity);
    if (direction == RIGHT)
        this->setPosition(this->getPosition() + this->getRight() * velocity);
    if (direction == BACKWARD)
        this->setPosition(this->getPosition() - this->getFront() * velocity);
    if (direction == FORWARD)
        this->setPosition(this->getPosition() + this->getFront() * velocity);
}

void Player::processRotation(const double yawRot, const double pitchRot, const double rollRot, const double deltaTime)
{
    MainCamera::get()->processRotation(yawRot, pitchRot, rollRot, deltaTime);
}

void Player::processScroll(const double dx, const double dy)
{
}

void Player::processScrollAlt(const double dx, const double dy)
{
    MainCamera::get()->processScroll(dx, dy);
}

void Player::processRotationAlt(double yawRot, double pitchRot, double rollRot, double deltaTime)
{

}