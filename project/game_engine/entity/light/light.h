#ifndef Light_H
#define Light_H

#include "../entity.h"

class Light : public Entity
{
    glm::vec3 _color = glm::vec3(1, 1, 1);

protected:
    Light(float x, float y, float z);
public:

    void setPosition(const glm::vec3& position) override;

    void setColor(const glm::vec3 &color);

    [[nodiscard]] glm::vec3 getColor() const;
};

#endif