
#include "controllable.h"

#include "../../utils/constants.h"

void Controllable::processMouseMovement(const double xoffset, const double yoffset, const double zoffset, const double deltaTime)
{
    this->processRotation(xoffset * SENSITIVITY, -yoffset * SENSITIVITY, zoffset * SENSITIVITY, deltaTime);
}

void Controllable::processMouseMovementAlt(const double xoffset, const double yoffset, const double zoffset, const double deltaTime)
{
    this->processRotationAlt(xoffset * SENSITIVITY, -yoffset * SENSITIVITY, zoffset * SENSITIVITY, deltaTime);
}

void Controllable::processRotationAlt(double yawRot, double pitchRot, double rollRot, double deltaTime) {  }

void Controllable::processScrollAlt(double dx, double dy) {  }
