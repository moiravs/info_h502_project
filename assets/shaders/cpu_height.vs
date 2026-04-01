#version 330 core
layout (location = 0) in vec3 aPos;

out float Height;
out vec3 Position;
out vec4 WorldPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 plane;

void main()
{
    WorldPos = vec4(model * vec4(aPos, 1.0));
    gl_ClipDistance[0] = dot(WorldPos, plane);

    Height = aPos.y;
    Position = (view * model * vec4(aPos, 1.0)).xyz;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}