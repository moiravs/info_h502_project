#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

out vec2 v_t;
void main()
{
    v_t = aTexCoords;
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);
}  