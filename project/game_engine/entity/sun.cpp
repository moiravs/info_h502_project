
#include "sun.h"

Sun::Sun(const std::shared_ptr<Mesh>& mesh, const float speed,
    const float orbitRadius, const std::shared_ptr<Renderer>& renderer)
    : Object(mesh, renderer), orbitSpeed(speed), orbitRadius(orbitRadius)
{}

void Sun::update(const float delta)
{
    float theta = std::atan2(this->getPosition().y, this->getPosition().z);

    theta += delta * orbitSpeed;

    constexpr float sunX = 0.0f;
    const float sunY = std::sin(theta) * orbitRadius;
    const float sunZ = std::cos(theta) * orbitRadius;

    setPosition(glm::vec3(sunX, sunY, sunZ));
}
