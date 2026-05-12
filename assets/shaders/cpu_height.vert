#version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec3 aNormal;

layout (std140) uniform WaterData {
    vec4 plane;
};

out float Height;
out vec3 v_normal;   // Pass to fragment
out vec3 v_fragPos;  // Pass to fragment
out vec4 WorldPos;

out vec2 v_texCoord; // New output
uniform mat4 model;
uniform mat4 itM;

layout(std140) uniform CameraInfo {
    mat4 projection;
    mat4 iProj;
    mat4 view;
    mat4 iView;
    vec4 camPosition;
    vec4 camRight;
    vec4 camUp;
};

void main()
{
    WorldPos = model * vec4(aPos, 1.0);
    gl_ClipDistance[0] = dot(WorldPos, plane);

    Height = aPos.y;
    v_fragPos = vec3(WorldPos);
    v_texCoord = aPos.xz / 50.0;
    // Transform normal to world space
    v_normal = mat3(itM) * aNormal;

    //gl_Position = color;
    gl_Position = projection * view * WorldPos;
}
