#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <iostream>

#include "../entity/camera.h"
#include <GLFW/glfw3.h>
#include "../entity/object.h"

#include <glm/glm.hpp>
#include "../entity/player.h"

class DisplayManager
{
    GLFWwindow *window;

    float deltaTime = 0.0f;
    int useWireframe = 0;
    int displayGrayscale = 0;

    bool firstMouse = true;
    double lastTime;
    int nbFrames = 0;

    // timing
    float lastFrame = 0.0f;

    std::function<void()> fps;

    GLFWwindow *createWindow();

public:
    double lastX = 800;
    double lastY = 600;
    DisplayManager();

    void resizeViewport(int width, int height) const;

    void moveMouse(double xpos, double ypos);

    [[nodiscard]] bool shouldClose() const;

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);

    void update(std::shared_ptr<Player> object);

    void processInput(std::shared_ptr<Player> object) const;
};
#endif