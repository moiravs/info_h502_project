
#include "entity.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "../../utils/constants.h"
#include "../manager/idmanager.h"

Entity::Attachment::Attachment(const std::shared_ptr<Entity>& entity, const glm::vec3& offset)
{
    this->entity = entity;
    this->offset = offset;
}

Entity::Entity(glm::vec3 pos, glm::vec3 up): Entity(pos, 0, 0, up) {}

Entity::Entity(glm::vec3 pos, float yaw, float pitch, glm::vec3 up)
: id(IDManager::generateID()), _pos(pos), _up(up), _yaw(yaw), _pitch(pitch)
{
    this->Entity::updateRotation();
}

glm::vec3 Entity::getPosition() const
{
    return this->_pos;
}

float Entity::getPitch() const
{
    return this->_pitch;
}

float Entity::getYaw() const
{
    return this->_yaw;
}

void Entity::attach(const std::shared_ptr<Entity>& entity, const glm::vec3 offset)
{
    const auto a = Attachment(entity, offset);
    this->_attached.push_back(a);
    this->updatePositionAttached(a);
}

void Entity::updatePositionAttached(const Attachment& attachment) const
{
    const glm::mat3 rotation(this->_right, this->_up, this->_front);

    const glm::vec3 worldOffset = rotation * attachment.offset;

    attachment.entity->setPosition(this->_pos + worldOffset);
}

void Entity::setPosition(const float x, const float y, const float z)
{
    this->setPosition(glm::vec3(x, y, z));
}

void Entity::setPosition(const glm::vec3& position)
{
    this->_pos = position;

    for (const auto& a: this->_attached)
        this->updatePositionAttached(a);
}

glm::vec3 Entity::getFront() const
{
    return this->_front;
}

glm::vec3 Entity::getRight() const
{
    return this->_right;
}

glm::vec3 Entity::getUp() const
{
    return this->_up;
}

void Entity::updateRotation()
{
    if (this->_pitch > 89.0f)
        this->_pitch = 89.0f;
    if (this->_pitch < -89.0f)
        this->_pitch = -89.0f;

    _front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front.y = sin(glm::radians(_pitch));
    _front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
    _front = glm::normalize(_front);
    // also re-calculate the Right and Up vector
    _right = glm::normalize(glm::cross(_front, WORLD_UP)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    _up = glm::normalize(glm::cross(_right, _front));

    this->updateRotationAttached();
}

void Entity::updateRotationAttached() const
{
    for (const auto &a: _attached)
    {
        a.entity->forceSetRotation(_front, _up, _right, _yaw, _pitch);
        this->updatePositionAttached(a);
    }
}

void Entity::forceSetRotation(const glm::vec3& front, const glm::vec3& up, const glm::vec3& right,
    const float yaw, const float pitch)
{
    this->_front = front;
    this->_up = up;
    this->_right = right;
    this->_yaw = yaw;
    this->_pitch = pitch;

    this->updateRotationAttached();
}

void Entity::rotate(const float dyaw, const float dpitch)
{
    this->setRotation(this->_yaw + dyaw, this->_pitch + dpitch);
}

void Entity::setRotation(const float yaw, const float pitch)
{
    this->_yaw = yaw;
    this->_pitch = pitch;

    this->updateRotation();
}

bool Entity::operator==(const Entity& other) const
{
    return this->id == other.id;
}
