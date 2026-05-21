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
    int pad1;
    int pad2;
    int pad3;
};

in vec3 TexCoords;
uniform samplerCube skybox;

void main()
{    
    vec4 originalColor = texture(skybox, TexCoords);
    
    if (sunDir.y > 0.0){
        float fadeFactor = clamp(sunDir.y * 10.0, 0.0, 1.0);
        gColor = mix(originalColor, vec4(0.0, 0.0, 0.0, 1.0), fadeFactor);
    } else {
        gColor = originalColor;
    }
    
    gNormal = vec3(0.0);
}