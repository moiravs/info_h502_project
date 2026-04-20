#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <iostream>

#include "camera.h"
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

// camera - give pretty starting point
inline Camera camera(glm::vec3(0.0, 0.0, 0.1));

// Camera camera = Camera(glm::vec3(140.0f, 110.0f, 120.0f),
//                        glm::vec3(0.0f, 1.0f, 0.0f),
//                        -0.0f, -0.0f);

class DisplayManager
{
    GLFWwindow *window;

    float deltaTime = 0.0f;
    int useWireframe = 0;
    int displayGrayscale = 0;

    double lastX = 800;
    double lastY = 600;
    bool firstMouse = true;
    double lastTime;
    int nbFrames = 0;

    // timing
    float lastFrame = 0.0f;

    std::function<void()> fps;

    GLFWwindow *createWindow();

public:
    DisplayManager();

    void resizeViewport(int width, int height) const;

    void moveMouse(double xpos, double ypos);

    bool shouldClose() const;

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);

    void update();

    void processInput() const;
};
#endif