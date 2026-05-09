

#version 330 core

layout(location = 0) out vec4 gColor;
layout(location = 1) out vec3 gNormal;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    gColor = texture(skybox, TexCoords);
}

