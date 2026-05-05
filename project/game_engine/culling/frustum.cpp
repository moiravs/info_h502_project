
#include "frustum.h"

#include "../entity/camera.h"

Frustum::Frustum(const Plane& near, const Plane& far, const Plane& left,
        const Plane& right, const Plane& bottom, const Plane& top)
: topFace(top), bottomFace(bottom), rightFace(right), leftFace(left), farFace(far), nearFace(near)
{}

Plane::Plane(const glm::vec3& point, const glm::vec3& normal): _normal(glm::normalize(normal)), _point(point)
{}

bool Plane::isInside(const glm::vec3& point) const
{
    return glm::dot(this->_normal, point - this->_point) >= 0.01f;
}

SignedTest Frustum::isInside(const std::array<glm::vec3, 8>& bounds) const
{
    bool intersects = false;

    for (const auto& plane : {topFace, bottomFace, rightFace, leftFace, farFace, nearFace})
    {
        bool allOutside = true;
        bool allInside = true;

        for (const auto& point : bounds)
        {
            if (plane.isInside(point))
            {
                allOutside = false;
            }
            else
            {
                allInside = false;
            }

            if (!allOutside && !allInside)
                break;
        }

        if (allOutside)
            return Outside;

        if (!allInside)
            intersects = true;
    }

    if (intersects)
        return Intersects;

    return Inside;
}

Plane::Plane(): Plane(glm::vec3(0), glm::vec3(0, 1, 0)) {}
