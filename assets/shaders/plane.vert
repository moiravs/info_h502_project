#version 420 core
in vec3 position;
in vec2 texcoord;
out vec2 v_tex; 

 void main(){
gl_Position = vec4(position, 1);
v_tex = texcoord;
}