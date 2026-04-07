#version 410 core
out vec4 FragColor;
precision mediump float; 

uniform sampler2D texture0;
in vec4 v_col; 
in vec2 v_t; 
void main() { 
    FragColor = texture(texture0, v_t);
} 