
#ifndef INFOH502_CPP_ENTITY_H
#define INFOH502_CPP_ENTITY_H

#include "../../utils/constants.h"
#include "glm/glm.hpp"

class Entity : public std::enable_shared_from_this<Entity> {
    struct Attachment
    {
        std::shared_ptr<Entity> entity;
        glm::vec3 offset{};
        Attachment() = default;
        Attachment(const std::shared_ptr<Entity>& entity, const glm::vec3& offset);
    };

    std::vector<Attachment> _attached = {};
    glm::vec3 _pos = glm::vec3(0,0,0);
    glm::vec3 _front = glm::vec3(1,0,0);
    glm::vec3 _up = glm::vec3(0, 0, 0);
    glm::vec3 _right = glm::vec3(0, 0, 0);
    float _yaw;
    float _pitch;

    void forceSetRotation(const glm::vec3 &front, const glm::vec3 &up, const glm::vec3 &right, float yaw, float pitch);
    inline void updateRotationAttached() const;
    inline void updatePositionAttached(const Attachment& attachment) const;
protected:
    virtual void updateRotation();

public:
    virtual ~Entity() = default;
    explicit Entity(glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 up = WORLD_UP);
    Entity(glm::vec3 pos, float yaw, float pitch, glm::vec3 up = WORLD_UP);

    void attach(const std::shared_ptr<Entity>& entity, glm::vec3 offset = glm::vec3(0, 0, 0));
    [[nodiscard]] glm::vec3 getPosition() const;
    virtual void setPosition(const glm::vec3& position);
    virtual void setPosition(float x, float y, float z);

    [[nodiscard]] glm::vec3 getUp() const;
    [[nodiscard]] glm::vec3 getRight() const;
    [[nodiscard]] glm::vec3 getFront() const;
    [[nodiscard]] float getYaw() const;
    [[nodiscard]] float getPitch() const;

    void rotate(float dyaw, float dpitch);
    void setRotation(float yaw, float pitch);
};



#endif //INFOH502_CPP_ENTITY_H
