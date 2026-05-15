#version 410 core

layout(location = 0) out vec4 lColor;

in vec2 TexCoords; 

uniform sampler2D color;
uniform sampler2D normal;
uniform sampler2D depth;
uniform sampler2D material;
uniform sampler2D shadow;
uniform vec2 viewport_size;

layout(std140) uniform Lights {
    vec4 lightPositions[32];
    vec4 lightProperties[32];   
    vec4 lightAttenuations[32]; 
    vec4 lightColors[32];
    mat4 sunPV;
    int lightCount;
    int pad1;
    int pad2;
    int pad3;
};

layout(std140) uniform CameraInfo {
    mat4 projection;
    mat4 iProj;
    mat4 view;
    mat4 iView;
    vec4 camPosition;
    vec4 camRight;
    vec4 camUp;
};

vec3 get_ray_direction() {
    vec2 ndc = (gl_FragCoord.xy / viewport_size) * 2.0 - 1.0;
    vec4 clip = vec4(ndc, -1.0, 1.0);
    vec4 view_ray = iProj * clip;
    view_ray = vec4(view_ray.xy, -1.0, 0.0);
    return normalize(vec3(iView * view_ray));
}

float linearize(float depthVal) {
    float near = 0.1; 
    float far = 5000.0; 
    float z = depthVal * 2.0 - 1.0; 
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
    float rawDepth = texture(depth, TexCoords).r;
    bool isSky = (rawDepth >= 1.0);
    
    vec3 rayDir = get_ray_direction();
    vec3 rawSceneColor = texture(color, TexCoords).rgb; 
    
    float dist = isSky ? 5000.0 : linearize(rawDepth);

    vec3 sunDir = normalize(vec3(sunPV[0][2], sunPV[1][2], sunPV[2][2])); 

    float fogDensity = 0.0004; 
    float heightFalloff = exp(-camPosition.y * 0.002);
    float extinction = exp(-dist * fogDensity * heightFalloff);

    vec3 finalOutput;

    vec3 fogScattering = rawSceneColor * (1.0 - extinction);
        
    finalOutput = fogScattering;

    finalOutput = finalOutput / (finalOutput + vec3(1.0));
    finalOutput = pow(finalOutput, vec3(1.0 / 2.2)); 

    lColor = vec4(finalOutput, 1.0);
}
