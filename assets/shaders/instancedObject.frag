#version 410 core

layout(location = 0) out vec4 gColor;
layout(location = 1) out vec3 gNormal;

layout(std140) uniform CameraInfo {
    mat4 projection;
    mat4 view;
    vec4 camPosition;
    vec4 camRight;
    vec4 camUp;
};

in vec2 v_t; 
in vec3 v_normal;
in vec3 v_fragPos;

uniform sampler2D texture0;

void main() {
    vec4 texColor = texture(texture0, v_t);
    if(texColor.a < 0.1) {
        discard;
    }

    vec3 norm = normalize(v_normal);
    vec3 totalLighting = vec3(0.0);

    gColor = texColor.rgba;
    gNormal = norm;
}