#version 420 core
in vec3 position; 
in vec2 tex_coord; 
in vec3 normal; 
out vec4 v_col; 
out vec2 v_t; 
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

    void main(){ 
gl_Position = projection*view*model*vec4(position, 1);
v_col = vec4(normal*0.5 + 0.5, 1.0);
v_t = tex_coord; 
}