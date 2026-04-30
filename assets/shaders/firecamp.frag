#version 410 core
out vec4 FragColor;

#define MAX_LIGHTS 128
layout(std140) uniform Lights {
    vec4 lightPositions[MAX_LIGHTS];
    vec4 lightProperties[MAX_LIGHTS];
    vec4 lightAttenuations[MAX_LIGHTS];
    vec4 lightColors[MAX_LIGHTS];
    int lightCount;
    int pad1;
    int pad2;
    int pad3;
};

layout(std140) uniform CameraInfo {
    mat4 projection;
    mat4 view;
    vec4 camPosition;
    vec4 camRight;
    vec4 camUp;
};

in vec2 v_t; 
in vec3 v_normal;
in vec3 v_fragPos;

uniform sampler2D shadowMap;
in vec4 FragPosLightSpace;   


float ShadowCalculation(vec4 fragPosLS) {
    vec3 projCoords = fragPosLS.xyz / fragPosLS.w * 0.5 + 0.5;
    

    
    float closest = texture(shadowMap, projCoords.xy).r;
    float current = projCoords.z;
    
    // 3. DEPTHS IDENTICAL? (Main suspect)
    if (abs(current - closest) < 0.001) {
        return 0.75;  // YELLOW = depths match exactly
    }
    
    // 4. ACTUALLY IN SHADOW?
    float bias = 0.001;
    return (current > closest + bias) ? 1.0 : 0.0;  // Red=shadow, Green=lit
}

uniform sampler2D texture0;

void main() {
    vec4 texColor = texture(texture0, v_t);
    if(texColor.a < 0.1) {
        discard;
    }

    vec3 norm = normalize(v_normal);
    vec3 totalLighting = vec3(0.0);


    // Calculate shadow once for the Sun
    float shadow = ShadowCalculation(FragPosLightSpace);

    for (int i = 0; i < lightCount; i++)
    {
        vec3 lightPos = lightPositions[i].xyz;
        vec3 lightColor = lightColors[i].xyz;
        
        vec3 ambient = lightProperties[i].x * lightColor;

        vec3 toLight = lightPos - v_fragPos;
        float dist = length(toLight);
        vec3 lightDir = toLight / max(dist, 0.0001);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightProperties[i].y * lightColor;

        vec3 att = lightAttenuations[i].xyz;
        float attenuation = 1.0 / (att.x + att.y * dist + att.z * dist * dist);

        if (i == 0) {
            totalLighting += (ambient + (1.0 - shadow) * diffuse) * attenuation;
        } else {
            totalLighting += (ambient + diffuse) * attenuation;
        }
    }


FragColor = vec4(shadow * 0.5, (1.0-shadow) * 0.5, shadow * 0.5 + 0.5, 1.0);



}