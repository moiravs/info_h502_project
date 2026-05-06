#include "player.h"

#include <iostream>

#include "../../utils/utils.h"
#include "glm/gtc/type_ptr.hpp"

Player::Player(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Renderer>& renderer): Object(mesh, renderer){}

void Player::processKeyboardMovement(const MovementDirection direction, const float deltaTime)
{
    printVec3(this->getFront());
    printVec3(this->getRight());
    printVec3(this->getUp());

    const float velocity = MOV_SPEED * deltaTime;
    if (direction == LEFT)
        this->setPosition(this->getPosition() - this->getRight() * velocity);
    if (direction == RIGHT)
        this->setPosition(this->getPosition() + this->getRight() * velocity);
    if (direction == BACKWARD)
        this->setPosition(this->getPosition() - this->getFront() * velocity);
    if (direction == FORWARD)
        this->setPosition(this->getPosition() + this->getFront() * velocity);
    if (direction == CROUCH)
        this->setPosition(this->getPosition() - WORLD_UP * velocity);
    if (direction == JUMP)
        this->setPosition(this->getPosition() + WORLD_UP * velocity);
}

void Player::processKeyboardRotation(const float yawRot, const float pitchRot, const float rollRot, const float deltaTime)
{
    const float velocity = ROT_SPEED * deltaTime;
    this->rotate(velocity * yawRot, velocity * pitchRot, velocity * rollRot);
}

void Player::processMouseMovement(const float xoffset, const float yoffset, const float zoffset, const float deltaTime)
{
    this->processKeyboardRotation(xoffset * SENSITIVITY, -yoffset * SENSITIVITY, zoffset * SENSITIVITY, deltaTime);
}
