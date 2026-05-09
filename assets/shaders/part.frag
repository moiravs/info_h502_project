#version 330 core

layout(location = 0) out vec4 gColor;
layout(location = 1) out vec3 gNormal;

in vec4 color;

void main(){
	gColor = color.rgba;
}