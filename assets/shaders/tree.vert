#version 410 core
in vec3 position;
in vec2 tex_coord;
in vec3 normal;
in mat4 model;

out vec2 v_t; 
out vec3 v_normal;
out vec3 v_fragPos;

uniform mat4 lightSpaceMatrix;

out vec4 FragPosLightSpace;

layout(std140) uniform CameraInfo {
    mat4 projection;
    mat4 view;
    vec4 camPosition;
    vec4 camRight;
    vec4 camUp;
};

void main() { 
    // Calculate world position
    vec4 worldPos = model * vec4(position, 1.0);
    v_fragPos = vec3(worldPos);
    
    // Transform normal to world space (remove scaling/translation influence)
    v_normal = mat3(transpose(inverse(model))) * normal;
    
    v_t = tex_coord; 
    gl_Position = projection * view * worldPos;
    FragPosLightSpace = lightSpaceMatrix * model * vec4(position, 1.0);
}
