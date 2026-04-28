
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
    virtual void processKeyboardMovement(MovementDirection direction, float deltaTime)=0;
    virtual void processKeyboardRotation(float yawRot, float pitchRot, float rollRot, float deltaTime)=0;
    virtual void processMouseMovement(float xoffset, float yoffset, float zoffset, float deltaTime)=0;
};



#endif //INFOH502_CPP_CONTROLLABLE_H
