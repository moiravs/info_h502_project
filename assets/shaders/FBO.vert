#version 330 core
in vec3 position;

uniform vec2 iResolution;
void main(){
	gl_Position = vec4(position, 1);
}