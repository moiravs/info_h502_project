#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main() {
    // Optional: Make the water physically bob
    float wave = sin(position.x * 0.5 + time) * 0.1;
    gl_Position = projection * view * model * vec4(position.x, position.y + wave, position.z, 1.0);
    TexCoord = tex_coord;
}