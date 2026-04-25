#version 330 core


// Uniforms sent from ObjectRenderer::updateUniforms
in vec3 position;
in vec2 tex_coord;
in vec3 normal;
in mat4 model;

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

void main()
{
    v_fragPos = vec3(model * vec4(position, 1.0));
    v_normal = mat3(transpose(inverse(model))) * normal;
    v_t = tex_coord;

    gl_Position = projection * view * vec4(v_fragPos, 1.0);
}