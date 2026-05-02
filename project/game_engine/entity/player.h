#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"
#include "camera.h"

// Defines several possible options for player movement. Used as abstraction to stay away from window-system specific input methods

// An abstract player class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Player : public Object, public Controllable
{
public:
    explicit Player(const std::shared_ptr<Mesh> &mesh, const std::shared_ptr<Renderer> &renderer);

    void processKeyboardMovement(MovementDirection direction, float deltaTime) override;

    void processKeyboardRotation(float yawRot, float pitchRot, float rollRot, float deltaTime) override;

    void processMouseMovement(float xoffset, float yoffset, float zoffset, float deltaTime) override;
};

#endif