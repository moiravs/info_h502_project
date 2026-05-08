#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aNormal;

uniform vec2 iResolution;
void main(){
	gl_Position = vec4(aPos, 1);
}