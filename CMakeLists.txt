cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(infoh502-cpp LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE ON)


# Find packages
find_package(OpenGL REQUIRED)

# GLAD
add_library(glad STATIC 3rdParty/glad/src/glad.c)
set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)
target_include_directories(glad PUBLIC 3rdParty/glad/include)

# GLFW
find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    option(GLFW_BUILD_DOCS OFF)
    option(GLFW_BUILD_EXAMPLES OFF)
    option(GLFW_BUILD_TESTS OFF)
    add_subdirectory(3rdParty/glfw)
    target_include_directories(glad PUBLIC 3rdParty/glfw/include)
endif()

# Global includes
target_include_directories(glad INTERFACE 
    3rdParty/glad/include/
    3rdParty/glm/
    3rdParty/stb/
)

# Define paths as macros
add_compile_definitions(PATH_TO_OBJECTS="${CMAKE_CURRENT_SOURCE_DIR}/assets/models")
add_compile_definitions(PATH_TO_TEXTURE="${CMAKE_CURRENT_SOURCE_DIR}/assets/textures")
add_compile_definitions(PATH_TO_SRC="${CMAKE_CURRENT_SOURCE_DIR}/project")

add_compile_definitions(GLM_ENABLE_EXPERIMENTAL)

# Solutions (conditional)
file(GLOB_RECURSE ALL_PROJECT_SOURCES 
    "project/*.cpp" 
    "project/*.h" 
    "project/*.hpp"
)

# Use the discovered files to create your executable
add_executable(VRproject ${ALL_PROJECT_SOURCES})
target_link_libraries(VRproject PUBLIC OpenGL::GL glfw glad)

