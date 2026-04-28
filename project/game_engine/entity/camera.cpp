#include "camera.h"

#include <iostream>

#include "glm/gtc/type_ptr.hpp"

Camera::Camera(const glm::vec3 up, const float yaw, const float pitch)
    : Entity(yaw, pitch, up), UboProvider("CameraInfo", sizeof(CameraInfo)), zoom(ZOOM)
{
    this->Camera::updateRotation();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(this->getPosition(), this->getPosition() + this->getFront(), this->getUp());
}

glm::mat4 Camera::getProjectionMatrix(const float fov, const float ratio, const float near, const float far)
{
    return glm::perspective(fov, ratio, near, far);
}

float Camera::getZoom() const
{
    return this->zoom;
}

void Camera::invertPitch()
{
    this->setRotation(this->getYaw(), -this->getPitch());
}

void Camera::processKeyboardMovement(const PlayerMovement direction, const float deltaTime)
{
    const float velocity = MOV_SPEED * deltaTime;
    if (direction == FORWARD)
        this->setPosition(this->getPosition() + this->flatFront * velocity);
    if (direction == BACKWARD)
        this->setPosition(this->getPosition() - this->flatFront * velocity);
    if (direction == LEFT)
        this->setPosition(this->getPosition() - this->getRight() * velocity);
    if (direction == RIGHT)
        this->setPosition(this->getPosition() + this->getRight() * velocity);
    if (direction == CROUCH)
        this->setPosition(this->getPosition() - WORLD_UP * velocity);
    if (direction == JUMP)
        this->setPosition(this->getPosition() + WORLD_UP * velocity);
}

void Camera::prepareReflection(const int height)
{
    const auto pos = this->getPosition();
    this->setPosition(pos.x, 2.0f * static_cast<float>(height) - pos.y, pos.z);

    this->invertPitch();
}

void Camera::resetCameraAfterReflection(const int height)
{
    this->invertPitch();

    const auto pos = this->getPosition();
    this->setPosition(pos.x, 2.0f * static_cast<float>(height) - pos.y, pos.z);
}

void Camera::processKeyboardRotation(const float yawRot, const float pitchRot, const float deltaTime)
{
    const float velocity = ROT_SPEED * deltaTime;

    this->rotate(velocity * yawRot, velocity * pitchRot);
}

void Camera::processMouseMovement(const float xoffset, const float yoffset, const float deltaTime)
{
    this->processKeyboardRotation(xoffset * SENSITIVITY, -yoffset * SENSITIVITY, deltaTime);
}

void Camera::processMouseScroll(const float yoffset)
{
    zoom -= static_cast<float>(yoffset);
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

void Camera::updateRotation()
{
    this->Entity::updateRotation();
    this->flatFront = this->getFront();
    this->flatFront.y = 0;
    this->flatFront = glm::normalize(this->flatFront);
}

void Camera::updateUBO() const
{
    const CameraInfo i = {
        .projection = Camera::getProjectionMatrix(glm::radians(this->getZoom()),
                                                  static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 100000.0f),
        .view = this->getViewMatrix(),
        .cameraPos = glm::vec4(this->getPosition(), 0),
        .cameraRight = glm::vec4(this->getRight(), 0),
        .cameraUp = glm::vec4(this->getUp(), 0)};

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(i.projection));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(i.view));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec4), glm::value_ptr(i.cameraPos));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(i.cameraRight));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(i.cameraUp));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// In your Camera.cpp
void Camera::setLookAt(glm::vec3 target)
{
    // 1. Calculate the direction vector from camera to target
    glm::vec3 direction = glm::normalize(target - this->getPosition());

    // 2. Calculate Pitch (Vertical angle)
    // direction.y is the 'Opposite' side of the triangle
    float newPitch = glm::degrees(asin(direction.y));

    // 3. Calculate Yaw (Horizontal angle)
    // Using atan2 handles the signs of x and z correctly
    float newYaw = glm::degrees(atan2(direction.z, direction.x));

    // 4. Update the Camera's state using your existing function
    // This will trigger the math needed for getFront() and updateRotation()
    this->setRotation(newYaw, newPitch);
    this->updateRotation();
}