#version 410 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D tex;
uniform bool isDepth;

void main()
{
    if (!isDepth) {
        FragColor = texture(tex, TexCoords).rgba;
    } else {
        float d = (((texture(tex, TexCoords).r - 0.99) * 100) - 0.99) * 100;
        FragColor = vec4(vec3(d), 1);
    }
}