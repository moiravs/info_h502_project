
#include "entity.h"

#include <iostream>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

#include "../manager/idmanager.h"
#include "glm/detail/type_quat.hpp"
#include "glm/ext/quaternion_trigonometric.hpp"

Entity::Attachment::Attachment(const std::shared_ptr<Entity> &entity, const glm::vec3 &offset,
    const bool shouldRender, const bool followsRotation, const bool rotationLocked)
{
    this->entity = entity;
    this->offset = offset;
    this->shouldRender = shouldRender;
    this->followsRotation = followsRotation;
    this->rotationLocked = rotationLocked;
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

void Entity::attach(const std::shared_ptr<Entity> &entity, const glm::vec3 offset,
    const bool shouldRender, const bool followsRotation, const bool rotationLocked)
{
    const auto a = std::make_shared<Attachment>(entity, offset, shouldRender, followsRotation, rotationLocked);
    this->_attached.push_back(a);
    this->updatePositionAttached(a);
}

void Entity::updatePositionAttached(const std::shared_ptr<Attachment> &attachment) const
{
    if (attachment->followsRotation)
    {
        const glm::mat3 rotation(this->_right, this->_up, this->_front);
        attachment->entity->setPosition(this->_pos + rotation * attachment->offset);
    }
    else
        attachment->entity->setPosition(this->_pos + attachment->offset);
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

bool Entity::shouldClampPitch() const { return false; }

void Entity::updateRotation()
{
    if (this->shouldClampPitch())    // 1.57 ~= pi/2
        this->_pitch = glm::clamp(this->_pitch, -1.57f, 1.57f);
    if (this->_pitch > glm::pi<float>())
        this->_pitch -= 2 * glm::pi<float>();
    if (this->_pitch < -glm::pi<float>())
        this->_pitch += 2 * glm::pi<float>();
    if (this->_yaw > glm::pi<float>())
        this->_yaw -= 2 * glm::pi<float>();
    if (this->_yaw < -glm::pi<float>())
        this->_yaw += 2 * glm::pi<float>();
    if (this->_roll > glm::pi<float>())
        this->_roll -= 2 * glm::pi<float>();
    if (this->_roll < -glm::pi<float>())
        this->_roll += 2 * glm::pi<float>();

    const glm::quat qYaw   = glm::angleAxis(-_yaw,   glm::vec3(0, 1, 0));
    const glm::quat qPitch = glm::angleAxis(-_pitch, glm::vec3(1, 0, 0));
    const glm::quat qRoll  = glm::angleAxis(_roll,  glm::vec3(0, 0, 1));

    const glm::quat trans = glm::normalize(qYaw * qPitch * qRoll);
    _front = trans * glm::vec3(0, 0, 1);
    _right = trans * glm::vec3(-1, 0, 0);
    _up = trans * glm::vec3(0, 1, 0);

    for (const auto& a: this->_attached)
        this->updatePositionAttached(a);
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

void Entity::removeAttachment(const std::shared_ptr<Entity>& entity)
{
    auto it = this->_attached.begin();
    for (; it != this->_attached.end(); ++it)
    {
        if (entity.get() == it->get()->entity.get()) break;
    }

    if (it == this->_attached.end()) return;

    // Move found element to the end, then remove it
    std::iter_swap(it, this->_attached.end() - 1);
    this->_attached.pop_back();
}

std::shared_ptr<Entity::Attachment> Entity::getAttachment(const std::shared_ptr<Entity>& entity)
{
    for (auto& a: this->_attached)
    {
        if (a->entity.get() == entity.get())
        {
            return a;
        }
    }
    return nullptr;
}

const std::vector<std::shared_ptr<Entity::Attachment>>& Entity::getAttachments() const
{
    return this->_attached;
}
