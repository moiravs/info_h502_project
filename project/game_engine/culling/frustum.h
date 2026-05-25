
#ifndef INFOH502_CPP_FRUSTUM_H
#define INFOH502_CPP_FRUSTUM_H

#include <glm/glm.hpp>
#include <array>

enum SignedTest
{
    Inside,
    Outside,
    Intersects
};

class Camera;

// https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
class Plane
{
    // unit vector
    glm::vec3 _normal;

    // distance from origin to the nearest point in the plane
    glm::vec3 _point;
public:
    Plane(const glm::vec3& point, const glm::vec3& normal);
    Plane();
    [[nodiscard]] bool isInside(const glm::vec3& point) const;
};

class Frustum {
    Plane topFace;
    Plane bottomFace;

    Plane rightFace;
    Plane leftFace;

    Plane farFace;
    Plane nearFace;
public:
    Frustum(
        const Plane& near, const Plane& far, const Plane& left,
        const Plane& right, const Plane& bottom, const Plane& top);
    Frustum()=default;

    void setTop(const Plane& plane);
    void setBottom(const Plane& plane);
    void setRight(const Plane& plane);
    void setLeft(const Plane& plane);
    void setFar(const Plane& plane);
    void setNear(const Plane& plane);

    [[nodiscard]] SignedTest isInside(const std::array<glm::vec3, 8>& bounds) const;
};



#endif //INFOH502_CPP_FRUSTUM_H
