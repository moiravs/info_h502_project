
#ifndef INFOH502_CPP_CONTROLLABLE_H
#define INFOH502_CPP_CONTROLLABLE_H

enum MovementDirection
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    CROUCH,
    JUMP
};

class Controllable {
public:
    virtual ~Controllable() = default;
    virtual void processKeyboardMovement(MovementDirection direction, double deltaTime)=0;
    virtual void processScroll(double dx, double dy)=0;
    virtual void processScrollAlt(double dx, double dy);
    virtual void processRotation(double yawRot, double pitchRot, double rollRot, double deltaTime)=0;
    virtual void processRotationAlt(double yawRot, double pitchRot, double rollRot, double deltaTime);
    virtual void processMouseMovement(double xoffset, double yoffset, double zoffset, double deltaTime);
    virtual void processMouseMovementAlt(double xoffset, double yoffset, double zoffset, double deltaTime);
};



#endif //INFOH502_CPP_CONTROLLABLE_H
