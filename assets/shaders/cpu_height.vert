#version 410 core
in vec3 position;
in vec3 normal; 
layout (std140) uniform WaterData {
    vec4 plane;
};


out float Height;
out vec3 v_normal;   // Pass to fragment
out vec3 v_fragPos;  // Pass to fragment
out vec4 WorldPos;

out vec2 v_texCoord; // New output
uniform mat4 model;

layout(std140) uniform CameraInfo {
    mat4 projection;
    mat4 view;
    vec4 camPosition;
    vec4 camRight;
    vec4 camUp;
};

void main()
{
    WorldPos = model * vec4(position, 1.0);
    gl_ClipDistance[0] = dot(WorldPos, plane);

    Height = position.y;
    v_fragPos = vec3(WorldPos);
    v_texCoord = position.xz / 50.0;
    // Transform normal to world space
    v_normal = mat3(transpose(inverse(model))) * normal;

    //gl_Position = color;
    gl_Position = projection * view * WorldPos;
}
