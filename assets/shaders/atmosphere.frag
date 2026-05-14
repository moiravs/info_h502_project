#version 410 core

layout(location = 0) out vec4 lColor;

in vec2 TexCoords; 

uniform vec3 sunDir;
uniform sampler2D color;
uniform sampler2D normal;
uniform sampler2D depth;
uniform sampler2D material;
uniform sampler2D shadow;
uniform vec2 viewport_size;

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
    vec3 rayDir = get_ray_direction();
    vec3 texColor = texture(color, TexCoords).rgb;
    vec3 texNormal = texture(normal, TexCoords).rgb * 2.0 - 1.0; 
    float texRough = texture(material, TexCoords).r;
    float rawDepth = texture(depth, TexCoords).r;
    float dist = (rawDepth >= 1.0) ? 5000.0 : linearize(rawDepth);

    float dayFactor = smoothstep(-0.2, 0.2, sunDir.y);
    float nightFactor = 1.0 - dayFactor;

    vec3 zenithDay = vec3(0.1, 0.2, 0.6);
    vec3 horizonDay = vec3(0.5, 0.7, 0.9);
    vec3 zenithNight = vec3(0.005, 0.005, 0.02); 
    vec3 horizonNight = vec3(0.02, 0.03, 0.08); 

    vec3 zCol = mix(zenithNight, zenithDay, dayFactor);
    vec3 hCol = mix(horizonNight, horizonDay, dayFactor);
    
    vec3 skyColor = mix(hCol, zCol, max(rayDir.y, 0.0));

    float upFacing = max(texNormal.y, 0.0);
    vec3 ambientNightLight = vec3(0.03, 0.04, 0.07) * upFacing;
    
    vec3 modifiedScene = texColor + (ambientNightLight * nightFactor);
    modifiedScene += vec3(0.01) * nightFactor; 

    float fogDensity = 0.0004;
    float heightFalloff = exp(-camPosition.y * 0.002);
    float extinction = exp(-dist * fogDensity * heightFalloff);

    float sunCos = max(dot(rayDir, sunDir), 0.0);
    float g = 0.8;
    float mie = (1.0 - g*g) / pow(1.0 + g*g - 2.0 * g * sunCos, 1.5);
    
    vec3 sunColor = vec3(1.0, 0.9, 0.7) * dayFactor;
    vec3 moonColor = vec3(0.4, 0.5, 0.7) * nightFactor * 0.2;
    vec3 lightGlow = (sunColor + moonColor) * mie * 0.15;

    vec3 finalOutput = mix(skyColor, modifiedScene, extinction);
    finalOutput += lightGlow;

    finalOutput = finalOutput / (finalOutput + vec3(1.0));
    finalOutput = pow(finalOutput, vec3(1.0/2.2)); 

    lColor = vec4(finalOutput, 1.0);
}