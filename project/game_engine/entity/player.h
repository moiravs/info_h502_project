#ifndef PLAYER_H
#define PLAYER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "entity.h"
#include "../../utils/constants.h"
#include "../manager/uboProvider.h"
#include "object.h"
#include "camera.h"

// Defines several possible options for player movement. Used as abstraction to stay away from window-system specific input methods

// An abstract player class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Player : public Object
{

protected:
    void updateRotation() override;

public:
    // constructor with vectors

    void processKeyboardMovement(PlayerMovement direction, float deltaTime);

    void processKeyboardRotation(float yawRot, float pitchRot, float deltaTime);

    void processMouseMovement(float xoffset, float yoffset, float deltaTime);
};

#endif