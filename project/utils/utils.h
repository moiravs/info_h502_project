
#ifndef INFOH502_CPP_UTILS_H
#define INFOH502_CPP_UTILS_H

#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>

#define ERROR(msg)                                         \
    std::cerr << "[ERROR] " << __FILE__ << ":" << __LINE__ \
              << " (" << __func__ << ") - " << msg << std::endl;

#define FATAL(msg)                                                   \
    do                                                               \
    {                                                                \
        std::cerr << "[FATAL] " << __FILE__ << ":" << __LINE__       \
                  << " (" << __func__ << ") - " << msg << std::endl; \
        std::exit(EXIT_FAILURE);                                     \
    } while (0)

inline bool isInVolume(const glm::vec3& point, const glm::vec3& lower, const glm::vec3& upper)
{
    return
        point.x < upper.x && point.x >= lower.x &&
        point.y < upper.y && point.y >= lower.y &&
        point.z < upper.z && point.z >= lower.z;
}

#ifndef NDEBUG
inline void APIENTRY glDebugOutput(const GLenum source,
                                   const GLenum type,
                                   const unsigned int id,
                                   const GLenum severity,
                                   GLsizei length,
                                   const char *message,
                                   const void *userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        std::cout << "Source: API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        std::cout << "Source: Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        std::cout << "Source: Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        std::cout << "Source: Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        std::cout << "Source: Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        std::cout << "Source: Other";
        break;
    }
    std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        std::cout << "Type: Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        std::cout << "Type: Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        std::cout << "Type: Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        std::cout << "Type: Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        std::cout << "Type: Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        std::cout << "Type: Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        std::cout << "Type: Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        std::cout << "Type: Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        std::cout << "Type: Other";
        break;
    }
    std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        std::cout << "Severity: high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        std::cout << "Severity: medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        std::cout << "Severity: low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        std::cout << "Severity: notification";
        break;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}
#endif

#endif // INFOH502_CPP_UTILS_H
