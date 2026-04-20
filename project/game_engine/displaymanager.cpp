#include "displaymanager.h"

#include "mainCamera.h"
#include "../utils/constants.h"
#include "../utils/utils.h"

GLFWwindow* DisplayManager::createWindow()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *w = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL: Terrain CPU",
                                     nullptr, nullptr);

    if (w == nullptr)
    {
        glfwTerminate();
        FATAL("Failed to create GLFW window.");
    }

    glfwMakeContextCurrent(w);
    glfwSetFramebufferSizeCallback(w, framebuffer_size_callback);
    glfwSetCursorPosCallback(w, cursor_position_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(w, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetWindowUserPointer(w, this);

    return w;
}

DisplayManager::DisplayManager(): window(DisplayManager::createWindow())
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
    MainCamera::get()->processMouseMovement(dx, dy, deltaTime);
}

bool DisplayManager::shouldClose() const
{
    return glfwWindowShouldClose(this->window);
}

void DisplayManager::framebuffer_size_callback(GLFWwindow* window, const int width, const int height)
{
    const auto dm = static_cast<DisplayManager *>(glfwGetWindowUserPointer(window));
    dm->resizeViewport(width, height);
}

void DisplayManager::cursor_position_callback(GLFWwindow* window, const double xpos, const double ypos)
{
    const auto dm = static_cast<DisplayManager *>(glfwGetWindowUserPointer(window));
    dm->moveMouse(xpos, ypos);
}

void DisplayManager::update()
{
    glfwSwapBuffers(this->window);
    glfwPollEvents();

    const auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    this->processInput();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Measure speed
    const double currentTime = glfwGetTime();
    nbFrames++;
    if (currentTime - lastTime >= 1.0)
    { // If last prinf() was more than 1 sec ago
        // printf and reset timer
        printf("%f ms/frame\n", 1000.0 / static_cast<double>(nbFrames));
        nbFrames = 0;
        lastTime += 1.0;
    }
}

void DisplayManager::processInput() const
{
    const auto camera = MainCamera::get();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        camera->processKeyboardMovement(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->processKeyboardMovement(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->processKeyboardMovement(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->processKeyboardMovement(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera->processKeyboardMovement(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->processKeyboardMovement(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->processKeyboardMovement(JUMP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->processKeyboardMovement(CROUCH, deltaTime);

    // camera rotation
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera->processKeyboardRotation(1.0, 0.0, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera->processKeyboardRotation(-1.0, 0.0, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera->processKeyboardRotation(0.0, 1.0, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera->processKeyboardRotation(0.0, -1.0, deltaTime);
}
