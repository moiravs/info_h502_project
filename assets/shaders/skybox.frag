#version 330 core

layout(location = 0) out vec4 gColor;
layout(location = 1) out vec3 gNormal;

#define MAX_LIGHTS 128

layout(std140) uniform Lights {
    vec4 lightPositions[MAX_LIGHTS];
    vec4 lightProperties[MAX_LIGHTS];
    vec4 lightAttenuations[MAX_LIGHTS];
    vec4 lightColors[MAX_LIGHTS];
    mat4 sunPV;
    vec4 sunDir;
    int lightCount;
};

in vec3 TexCoords;
uniform samplerCube skybox;

void main()
{    
    gColor = texture(skybox, TexCoords);
    gNormal = vec3(0.0);
}