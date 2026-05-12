#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aNormal;

uniform mat4 model;

layout(std140) uniform CameraInfo {
    mat4 projection;
    mat4 iProj;
    mat4 view;
    mat4 iView;
    vec4 camPosition;
    vec4 camRight;
    vec4 camUp;
};

uniform float time;

out vec2 TexCoord;


out vec4 clipSpace;

const float tiling = 0.05;

void main() {

    clipSpace = projection * view * model * vec4(aPos.xyz, 1.0);
    gl_Position = clipSpace;

    TexCoord = vec2(aPos.x /2.0 + 0.5, aPos.z/2.0 + 0.5) * tiling;

}