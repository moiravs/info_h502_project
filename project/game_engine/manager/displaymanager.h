#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <iostream>

#include "../entity/camera.h"
#include <GLFW/glfw3.h>
#include "../entity/object.h"

#include <glm/glm.hpp>
#include "../entity/player.h"

enum class GameState
{
    MENU,
    PLAYING
};
class DisplayManager
{

    GLFWwindow *window;

    double deltaTime = 0.0f;
    int useWireframe = 0;
    int displayGrayscale = 0;

    bool firstMouse = true;
    double lastTime;
    int nbFrames = 0;

    int width;
    int height;
    bool pressedTLastFrame = false;

    // timing
    float lastFrame = 0.0f;

    std::function<void()> fps;

    GLFWwindow *createWindow();

public:
    GameState currentState = GameState::MENU;

    double lastX = 800;
    double lastY = 600;
    float frameTimeSeconds = 0;
    DisplayManager();

    void resizeViewport(int width, int height) const;

    void moveMouse(double xpos, double ypos);

    [[nodiscard]] bool shouldClose() const;

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);

    int getWidth() const;

    int getHeight() const;

    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

    // Add to public section of DisplayManager class
    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    void handleMouseButton(int button, int action, int mods);
    void update();

    void processInput(const std::shared_ptr<Controllable> &object);
};
#endif