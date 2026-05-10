#version 330 core
layout(location = 0) out vec4 gColor;
layout(location = 1) out vec3 gNormal;
precision mediump float; 

in vec3 v_normal;

uniform vec3 materialColour;

void main() {
    gColor = vec4(materialColour, 1);
    gNormal = v_normal;
}