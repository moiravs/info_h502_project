#include "camera.h"

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

glm::mat4 Camera::getProjectionMatrix(const float fov, const float ratio, const float near, const float far)
{
    return glm::perspective(fov, ratio, near, far);
}

glm::vec3 Camera::getUp() const
{
    return this->Up;
}

float Camera::getZoom() const
{
    return this->Zoom;
}

glm::vec3 Camera::getPosition() const
{
    return this->Position;
}

glm::vec3 Camera::getCameraRight() const
{
    return this->Right;
}

glm::vec3 Camera::getCameraPosition() const
{
    return this->Position;
}

void Camera::setCameraPosition(const glm::vec3 position)
{
    this->Position = position;
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    front.y = 0;
    FlatFront = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up = glm::normalize(glm::cross(Right, Front));
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch):
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), FlatFront(Front),
    MovementSpeed(MOV_SPEED), RotationSpeed(ROT_SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    this->Position = position;
    this->WorldUp = up;
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch):
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(MOV_SPEED), RotationSpeed(ROT_SPEED),
          MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
    this->Position = glm::vec3(posX, posY, posZ);
    this->WorldUp = glm::vec3(upX, upY, upZ);
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

void Camera::invertPitch()
{
    this->Pitch = -this->Pitch;
    updateCameraVectors();
}

void Camera::processKeyboardMovement(const PlayerMovement direction, const float deltaTime)
{
    const float velocity = this->MovementSpeed * deltaTime;
    if (direction == FORWARD)
        this->Position += this->FlatFront * velocity;
    if (direction == BACKWARD)
        this->Position -= this->FlatFront * velocity;
    if (direction == LEFT)
        this->Position -= this->Right * velocity;
    if (direction == RIGHT)
        this->Position += this->Right * velocity;
    if (direction == CROUCH)
        this->Position -= WORLD_UP * velocity;
    if (direction == JUMP)
        this->Position += WORLD_UP * velocity;
}

void Camera::prepareReflection(const int height)
{
    this->Position.y = 2.0f * static_cast<float>(height) - this->Position.y;

    this->invertPitch();
}

void Camera::resetCameraAfterReflection(const int height)
{
    this->invertPitch();

    this->Position.y = 2.0f * static_cast<float>(height) - this->Position.y;
}

void Camera::processKeyboardRotation(float YawRot, float PitchRot, const float deltaTime, const GLboolean constrainPitch)
{
    const float velocity = this->RotationSpeed * deltaTime;
    YawRot *= velocity;
    PitchRot *= velocity;

    this->Yaw += YawRot;
    this->Pitch += PitchRot;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::processMouseMovement(const float xoffset, const float yoffset, const float deltaTime, const GLboolean constrainPitch)
{
    this->processKeyboardRotation(xoffset * MouseSensitivity, -yoffset * MouseSensitivity,
                                      deltaTime, constrainPitch);
}

void Camera::processMouseScroll(const float yoffset)
{
    Zoom -= static_cast<float>(yoffset);
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}
