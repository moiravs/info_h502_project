#version 420 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

out vec2 TexCoord;


out vec4 clipSpace;

const float tiling = 20.0;

void main() {

    clipSpace = projection * view * model * vec4(position.x, position.y, position.z, 1.0);
    gl_Position = clipSpace;

    TexCoord = vec2(position.x /2.0 + 0.5, position.y/2.0 + 0.5) * tiling;
}