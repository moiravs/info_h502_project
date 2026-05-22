#version 410 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in mat4 aInstModel;

uniform mat4 PV;

void main()
{
    gl_Position = PV * aInstModel * vec4(aPos, 1.0);
}