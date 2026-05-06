
#include "entity.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "../manager/idmanager.h"
#include "glm/detail/type_quat.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"

Entity::Attachment::Attachment(const std::shared_ptr<Entity> &entity, const glm::vec3 &offset)
{
    this->entity = entity;
    this->offset = offset;
}

Entity::Entity(glm::vec3 up) : Entity(0, 0, 0, up) {}

Entity::Entity(float yaw, float pitch, float roll, glm::vec3 up)
    : id(IDManager::generateID()), _up(up), _yaw(yaw), _pitch(pitch), _roll(roll)
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

float Entity::getRoll() const
{
    return this->_roll;
}

void Entity::attach(const std::shared_ptr<Entity> &entity, const glm::vec3 offset)
{
    const auto a = Attachment(entity, offset);
    this->_attached.push_back(a);
    this->updatePositionAttached(a);
}

void Entity::updatePositionAttached(const Attachment &attachment) const
{
    const glm::mat3 rotation(this->_right, this->_up, this->_front);

    const glm::vec3 worldOffset = rotation * attachment.offset;

    attachment.entity->setPosition(this->_pos + worldOffset);
}

void Entity::setPosition(const float x, const float y, const float z)
{
    this->setPosition(glm::vec3(x, y, z));
}

void Entity::setPosition(const glm::vec3 &position)
{
    this->_pos = position;

    for (const auto &a : this->_attached)
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
    if (this->_pitch > 1.57) // ~pi/2
        this->_pitch = 1.57;
    if (this->_pitch < -1.57)
        this->_pitch = -1.57;
    if (this->_yaw > glm::pi<float>())
        this->_yaw -= 2 * glm::pi<float>();
    if (this->_yaw < -glm::pi<float>())
        this->_yaw += 2 * glm::pi<float>();
    if (this->_roll > glm::pi<float>())
        this->_roll -= 2 * glm::pi<float>();
    if (this->_roll < -glm::pi<float>())
        this->_roll += 2 * glm::pi<float>();

    glm::quat qYaw   = glm::angleAxis(-_yaw,   glm::vec3(0, 1, 0));
    glm::quat qPitch = glm::angleAxis(-_pitch, glm::vec3(1, 0, 0));
    glm::quat qRoll  = glm::angleAxis(_roll,  glm::vec3(0, 0, 1));

    glm::quat trans = glm::normalize(qYaw * qPitch * qRoll);
    _front = trans * glm::vec3(0, 0, 1);
    _right = trans * glm::vec3(-1, 0, 0);
    _up = trans * glm::vec3(0, 1, 0);

    // 4. Apply Roll
    if (abs(this->_roll) > 0.001f)
    {
        // Rotate the Up and Right vectors around the Front axis
        glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), this->_roll, _front);
        _right = glm::vec3(rollMatrix * glm::vec4(_right, 0.0f));
        _up = glm::vec3(rollMatrix * glm::vec4(_up, 0.0f));
    }

    this->updateRotationAttached();
}

void Entity::updateRotationAttached() const
{
    for (const auto &a : _attached)
    {
        a.entity->forceSetRotation(_front, _up, _right, _yaw, _pitch, _roll);
        this->updatePositionAttached(a);
    }
}

void Entity::forceSetRotation(const glm::vec3 &front, const glm::vec3 &up, const glm::vec3 &right,
                              const float yaw, const float pitch, const float roll)
{
    this->_front = front;
    this->_up = up;
    this->_right = right;
    this->_yaw = yaw;
    this->_pitch = pitch;
    this->_roll = roll;

    this->updateRotationAttached();
}

void Entity::rotate(const float dyaw, const float dpitch, const float droll)
{
    this->setRotation(this->_yaw + dyaw, this->_pitch + dpitch, this->_roll + droll);
}

void Entity::setRotation(const float yaw, const float pitch, const float roll)
{
    this->_yaw = yaw;
    this->_pitch = pitch;
    this->_roll = roll;

    this->updateRotation();
}

bool Entity::operator==(const Entity &other) const
{
    return this->id == other.id;
}

void Entity::update(const float delta)
{
}
