#include "player.h"

#include <iostream>

#include "glm/gtc/type_ptr.hpp"

void Player::processKeyboardMovement(const PlayerMovement direction, const float deltaTime)
{
    const float velocity = MOV_SPEED * deltaTime;
    if (direction == LEFT)
        this->setPosition(this->getPosition() + this->getFront() * velocity);
    if (direction == RIGHT)
        this->setPosition(this->getPosition() - this->getFront() * velocity);
    if (direction == BACKWARD)
        this->setPosition(this->getPosition() - this->getRight() * velocity);
    if (direction == FORWARD)
        this->setPosition(this->getPosition() + this->getRight() * velocity);
    if (direction == CROUCH)
        this->setPosition(this->getPosition() - WORLD_UP * velocity);
    if (direction == JUMP)
        this->setPosition(this->getPosition() + WORLD_UP * velocity);
}

void Player::processKeyboardRotation(const float yawRot, const float pitchRot, const float deltaTime)
{
    const float velocity = ROT_SPEED * deltaTime;

    this->rotate(velocity * yawRot, velocity * pitchRot);
}

void Player::processMouseMovement(const float xoffset, const float yoffset, const float deltaTime)
{
    this->processKeyboardRotation(xoffset * SENSITIVITY, -yoffset * SENSITIVITY, deltaTime);
}
