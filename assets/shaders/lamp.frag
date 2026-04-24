#version 330 core
out vec4 FragColor;
precision mediump float; 

uniform vec3 materialColour;

void main() {
    FragColor = vec4(materialColour, 1.0);
}