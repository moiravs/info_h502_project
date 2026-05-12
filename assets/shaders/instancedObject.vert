#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in mat4 aInstModel;

out vec2 v_t; 
out vec3 v_normal;
out vec3 v_fragPos;

out vec4 FragPosLightSpace;

layout(std140) uniform CameraInfo {
    mat4 projection;
    mat4 iProj;
    mat4 view;
    mat4 iView;
    vec4 camPosition;
    vec4 camRight;
    vec4 camUp;
};

void main() { 
    // Calculate world position
    vec4 worldPos = aInstModel * vec4(aPos, 1.0);
    v_fragPos = vec3(worldPos);
    
    // Transform normal to world space (remove scaling/translation influence)
    v_normal = mat3(transpose(inverse(aInstModel))) * aNormal;
    
    v_t = aTex;
    gl_Position = projection * view * worldPos;
}
