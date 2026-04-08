#version 410 core
layout (location = 0) in vec3 position; 
layout (location = 1) in vec2 tex_coord; 
layout (location = 2) in vec3 normal; 
// The instance matrix starts at location 3 and spans 3, 4, 5, 6
layout (location = 3) in mat4 instanceMatrix; 

out vec4 v_col; 
out vec2 v_t; 

uniform mat4 view;
uniform mat4 projection;

void main() { 
    // Use instanceMatrix instead of a single 'model' uniform
    gl_Position = projection * view * instanceMatrix * vec4(position, 1.0);
    v_col = vec4(normal * 0.5 + 0.5, 1.0);
    v_t = tex_coord; 
}
