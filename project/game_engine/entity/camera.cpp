#include "camera.h"

#include "../../utils/utils.h"
#include "glm/gtc/type_ptr.hpp"

Camera::Camera(const glm::vec3 up, const float yaw, const float pitch, const float roll)
    : Entity(yaw, pitch, roll, up), UboProvider("CameraInfo", sizeof(CameraInfo)), zoom(ZOOM),
    _frustum(std::make_shared<Frustum>())
{
    this->Camera::updateRotation();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(this->getPosition(), this->getPosition() + this->getFront(), this->getUp());
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(this->getFOV(), this->getAspectRatio(), NEAR, FAR);
}

float Camera::getZoom() const
{
    return this->zoom;
}

float Camera::getAspectRatio() const
{
    return ASPECT_RATIO;
}

void Camera::invertPitch()
{
    this->setRotation(this->getYaw(), -this->getPitch(), this->getRoll());
}

void Camera::processKeyboardMovement(const MovementDirection direction, const double deltaTime)
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
    this->lookingAt = false;
    const auto pos = this->getPosition();
    this->setPosition(pos.x, 2.0f * static_cast<float>(height) - pos.y, pos.z);

    this->invertPitch();
}

void Camera::resetCameraAfterReflection(const int height)
{
    this->invertPitch();

    this->lookingAt = true;
    const auto pos = this->getPosition();
    this->setPosition(pos.x, 2.0f * static_cast<float>(height) - pos.y, pos.z);
}

void Camera::processRotation(const double yawRot, const double pitchRot, const double rollRot, const double deltaTime)
{
    const double velocity = ROT_SPEED * deltaTime;

    this->rotate(velocity * yawRot, velocity * pitchRot, 0);
}

bool Camera::shouldClampPitch() const { return true; }

void Camera::updateRotation()
{
    this->Entity::updateRotation();
    this->flatFront = this->getFront();
    this->flatFront.y = 0;
    this->flatFront = glm::normalize(this->flatFront);

    this->updateFrustum();
}

float Camera::getFOV() const
{
    return glm::radians(this->getZoom());
}

// https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
void Camera::updateFrustum() const
{
    const float halfVSide = FAR * tanf(this->getFOV() * .5f);
    const float halfHSide = halfVSide * this->getAspectRatio();
    const glm::vec3 frontMultFar = FAR * this->getFront();
    const auto pos = this->getPosition();

    this->_frustum->setNear(Plane(pos + NEAR * this->getFront(), this->getFront()));
    this->_frustum->setFar(Plane(pos + frontMultFar, -this->getFront()));
    this->_frustum->setLeft(Plane(pos, glm::cross(frontMultFar - this->getRight() * halfHSide, this->getUp())));
    this->_frustum->setRight(Plane(pos, glm::cross(this->getUp(),frontMultFar + this->getRight() * halfHSide)));
    this->_frustum->setBottom(Plane(pos, glm::cross(this->getRight(), frontMultFar - this->getUp() * halfVSide)));
    this->_frustum->setTop(Plane(pos, glm::cross(frontMultFar + this->getUp() * halfVSide, this->getRight())));
}

void Camera::setPosition(const glm::vec3& position)
{
    Entity::setPosition(position);
    this->updateLook();
    this->updateFrustum();
}

void Camera::updateUBO() const
{
    const auto proj = this->getProjectionMatrix();
    const auto view = this->getViewMatrix();
    const CameraInfo i = {
        .projection = this->getProjectionMatrix(),
        .iProj = glm::inverse(proj),
        .view = this->getViewMatrix(),
        .iView = glm::inverse(view),
        .cameraPos = glm::vec4(this->getPosition(), 1),
        .cameraRight = glm::vec4(this->getRight(), 1),
        .cameraUp = glm::vec4(this->getUp(), 1)};

    glBindBuffer(GL_UNIFORM_BUFFER, ubo);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(i.projection));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(i.iProj));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(i.view));
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(i.iView));
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::mat4), sizeof(glm::vec4), glm::value_ptr(i.cameraPos));
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(i.cameraRight));
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(i.cameraUp));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::lookAt(const glm::vec3& target)
{
    const glm::vec3 delta = target - this->getPosition();
    const float len = glm::length(delta);

    if (len < 1e-6f)
        return;

    const glm::vec3 direction = delta / len;

    const float newPitch = asin(glm::clamp(direction.y, -1.0f, 1.0f));

    const float newYaw = atan2(-direction.x, direction.z);

    this->setRotation(newYaw, newPitch, 0.0f);
}

bool Camera::canView(const std::array<glm::vec3, 8>& bounds) const {
    const auto ret = this->_frustum->isInside(bounds) ;
    return ret == Inside || ret == Intersects;
}

void Camera::processScroll(const double dx, const double dy)
{
    this->zoom += dy;

    if (this->zoom < MIN_ZOOM)
        this->zoom = MIN_ZOOM;

    if (this->zoom > MAX_ZOOM)
        this->zoom = MAX_ZOOM;
}

void Camera::forceLookAt(const std::shared_ptr<Entity> &entity)
{
    this->lockedEntity = entity;
    this->updateLook();
}

void Camera::updateLook()
{
    if (!this->lockedEntity || !this->lookingAt) return;

    this->lookAt(this->lockedEntity->getPosition());
}

std::shared_ptr<Frustum> Camera::getFrustum() const
{
    if (!this->_frustum)
    {
        FATAL("No frustum yet.");
    }
    return this->_frustum;
}

std::vector<glm::vec3> Camera::getFrustumCorners() const
{
    const glm::mat4 inv = glm::inverse(this->getProjectionMatrix() * this->getViewMatrix());

    std::vector<glm::vec3> corners;

    // generate all corners of the projection cube (-1,-1,-1)...(+1,+1,+1)
    // and apply the inverse of PV to get the world coordinates.
    // this works to find the world coordinates of the corners since
    // PV (world coords) maps to said cube
    for (int x = 0; x < 2; ++x)
    {
        for (int y = 0; y < 2; ++y)
        {
            for (int z = 0; z < 2; ++z)
            {
                glm::vec4 pt = inv * glm::vec4(
                    2.0f * x - 1.0f,
                    2.0f * y - 1.0f,
                    2.0f * z - 1.0f,
                    1.0f
                );

                corners.push_back(glm::vec3(pt / pt.w));
            }
        }
    }

    return corners;
}

glm::mat4 Camera::getPV() const
{
    return this->getProjectionMatrix() * this->getViewMatrix();
}
