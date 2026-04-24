#version 410 core
layout (location = 0) in vec3 position; 
layout (location = 1) in vec2 tex_coord; 
layout (location = 2) in vec3 normal; 
layout (location = 3) in mat4 instanceMatrix; 

out vec2 v_t; 
out vec3 v_normal;
out vec3 v_fragPos;

layout(std140) uniform CameraInfo {
    mat4 projection;
    mat4 view;
    vec4 camPosition;
    vec4 camRight;
    vec4 camUp;
};

void main() { 
    // Calculate world position
    vec4 worldPos = instanceMatrix * vec4(position, 1.0);
    v_fragPos = vec3(worldPos);
    
    // Transform normal to world space (remove scaling/translation influence)
    v_normal = mat3(transpose(inverse(instanceMatrix))) * normal;
    
    v_t = tex_coord; 
    gl_Position = projection * view * worldPos;
}
