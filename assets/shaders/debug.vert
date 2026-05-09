#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aNormal;

out vec2 TexCoords;
void main() {
    TexCoords = aTex;
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}