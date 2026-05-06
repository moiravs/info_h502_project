
#ifndef INFOH502_CPP_ENTITY_H
#define INFOH502_CPP_ENTITY_H

#include "../../utils/constants.h"
#include "glm/glm.hpp"
#include <memory>

class Entity : public std::enable_shared_from_this<Entity>
{
protected:
    struct Attachment
    {
        std::shared_ptr<Entity> entity;
        glm::vec3 offset{};
        Attachment() = default;
        Attachment(const std::shared_ptr<Entity> &entity, const glm::vec3 &offset);
    };

    virtual void updateRotation();
    std::shared_ptr<Attachment> getAttachment(const std::shared_ptr<Entity> &entity);
    void updatePositionAttached(const std::shared_ptr<Attachment> &attachment) const;
    virtual bool shouldClampPitch() const;

private:
    size_t id;
    std::vector<std::shared_ptr<Attachment>> _attached = {};
    glm::vec3 _pos = glm::vec3(0, 0, 0);
    glm::vec3 _front = glm::vec3(1, 0, 0);
    glm::vec3 _up = glm::vec3(0, 0, 0);
    glm::vec3 _right = glm::vec3(0, 0, 0);
    float _yaw;
    float _pitch;
    float _roll;
    float _lastTime;
public:
    virtual ~Entity() = default;
    explicit Entity(glm::vec3 up = WORLD_UP);
    Entity(float yaw, float pitch, float roll, glm::vec3 up = WORLD_UP);

    void attach(const std::shared_ptr<Entity> &entity, glm::vec3 offset = glm::vec3(0, 0, 0));
    [[nodiscard]] glm::vec3 getPosition() const;
    virtual void setPosition(const glm::vec3 &position);
    virtual void setPosition(float x, float y, float z);

    [[nodiscard]] glm::vec3 getUp() const;
    [[nodiscard]] glm::vec3 getRight() const;
    [[nodiscard]] glm::vec3 getFront() const;
    [[nodiscard]] float getYaw() const;
    [[nodiscard]] float getPitch() const;
    [[nodiscard]] float getRoll() const;

    void rotate(float dyaw, float dpitch, float droll);
    virtual void setRotation(float yaw, float pitch, float roll);
    virtual void update(float delta);
    void removeAttachment(const std::shared_ptr<Entity>& entity);

    bool operator==(const Entity &other) const;
};

#endif // INFOH502_CPP_ENTITY_H
