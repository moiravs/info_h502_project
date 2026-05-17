#include "displaymanager.h"

#include "../../utils/constants.h"
#include "../../utils/utils.h"
#include "../entity/object.h"
#include "mainCamera.h"
#include "controllerManager.h"

GLFWwindow *DisplayManager::createWindow()
{
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *w = glfwCreateWindow(mode->width, mode->height, "3D Game",
                                     monitor, nullptr);

    width = mode->width;
    height = mode->height;
    if (w == nullptr)
    {
        glfwTerminate();
        FATAL("Failed to create GLFW window.");
    }

    glfwMakeContextCurrent(w);
    glfwSetFramebufferSizeCallback(w, framebuffer_size_callback);
    glfwSetCursorPosCallback(w, cursor_position_callback);
    glfwSetScrollCallback(w, scroll_callback);
    glfwSetMouseButtonCallback(w, mouse_button_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(w, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetWindowUserPointer(w, this);

    return w;
}

DisplayManager::DisplayManager() : window(DisplayManager::createWindow())
{
    lastTime = glfwGetTime();
}

void DisplayManager::resizeViewport(const int width, const int height) const
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    int framebuffer_width, framebuffer_height;
    glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
    glViewport(0, 0, framebuffer_width, framebuffer_height);
}

void DisplayManager::moveMouse(const double xpos, const double ypos)
{
    if (firstMouse)
    {
        this->lastX = xpos;
        this->lastY = ypos;
        firstMouse = false;
    }

    const auto dx = static_cast<float>(xpos - this->lastX);
    const auto dy = static_cast<float>(ypos - this->lastY);

    this->lastX = xpos;
    this->lastY = ypos;

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        ControllerManager::get()->getMainControllable()->processMouseMovementAlt(dx, dy, 0, deltaTime);
    else
        ControllerManager::get()->getMainControllable()->processMouseMovement(dx, dy, 0, deltaTime);
}

bool DisplayManager::shouldClose() const
{
    return glfwWindowShouldClose(this->window);
}

void DisplayManager::framebuffer_size_callback(GLFWwindow *window, const int width, const int height)
{
    const auto dm = static_cast<DisplayManager *>(glfwGetWindowUserPointer(window));
    dm->resizeViewport(width, height);
}

void DisplayManager::cursor_position_callback(GLFWwindow *window, const double xpos, const double ypos)
{
    const auto dm = static_cast<DisplayManager *>(glfwGetWindowUserPointer(window));

    dm->moveMouse(xpos, ypos);
}

int DisplayManager::getWidth() const
{
    return width;
}

int DisplayManager::getHeight() const
{
    return height;
}

void DisplayManager::scroll_callback(GLFWwindow *window, const double xoffset, const double yoffset)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        ControllerManager::get()->getMainControllable()->processScrollAlt(xoffset, yoffset);
    else
        ControllerManager::get()->getMainControllable()->processScroll(xoffset, yoffset);
}

void DisplayManager::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    // Route the static call to the class instance
    const auto dm = static_cast<DisplayManager *>(glfwGetWindowUserPointer(window));
    dm->handleMouseButton(button, action, mods);
}

void DisplayManager::handleMouseButton(int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (currentState == GameState::MENU)
        {
            // if (xpos > 300 && xpos < 500 && ypos > 250 && ypos < 350)
            // {
            currentState = GameState::PLAYING;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            // }
        }
    }
}

void DisplayManager::update()
{
    glfwSwapBuffers(this->window);
    glfwPollEvents();

    const double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    this->processInput(ControllerManager::get()->getMainControllable());

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Measure speed
    const double currentTime = glfwGetTime();
    nbFrames++;
    frameTimeSeconds = 1000.0 / static_cast<double>(nbFrames);
    if (currentTime - lastTime >= 1.0)
    { // If last prinf() was more than 1 sec ago
        // printf and reset timer
        printf("%f ms/frame\n", frameTimeSeconds);
        nbFrames = 0;
        lastTime += 1.0;
    }
}

void DisplayManager::processInput(const std::shared_ptr<Controllable> &object)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        object->processKeyboardMovement(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        object->processKeyboardMovement(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        object->processKeyboardMovement(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        object->processKeyboardMovement(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        object->processKeyboardMovement(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        object->processKeyboardMovement(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        object->processKeyboardMovement(JUMP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        object->processKeyboardMovement(CROUCH, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        if (!this->pressedTLastFrame)
            ControllerManager::get()->toggleIsPlayerControlled();

        this->pressedTLastFrame = true;
    }
    else
    {
        this->pressedTLastFrame = false;
    }

    // camera rotation
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            object->processRotationAlt(1.0, 0.0, 0.0, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            object->processRotationAlt(-1.0, 0.0, 0.0, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            object->processRotationAlt(0.0, 1.0, 0.0, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            object->processRotationAlt(0.0, -1.0, 0.0, deltaTime);
    }
    else
    {
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            object->processRotation(1.0, 0.0, 0.0, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            object->processRotation(-1.0, 0.0, 0.0, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            object->processRotation(0.0, 1.0, 0.0, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            object->processRotation(0.0, -1.0, 0.0, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        object->processRotation(0.0, 0.0, -1.0, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        object->processRotation(0.0, 0.0, 1.0, deltaTime);
}
