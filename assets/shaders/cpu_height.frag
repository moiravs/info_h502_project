#version 410 core

out vec4 FragColor;

#define MAX_LIGHTS 128

layout(std140) uniform Mist {
vec3 mistColor;
float mistDensity;
vec3 cameraPos;
float fogMaxHeight;  
float fogMinHeight;   
float fogDensity;  
};


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

in float Height;
in vec3 v_normal;
in vec3 v_fragPos;

in vec2 v_texCoord;
uniform sampler2D grassTex;
uniform sampler2D rockTex;
uniform sampler2D snowTex;

uniform sampler2D shadowMap; 
in vec4 FragPosLightSpace;  

float ShadowCalculation(vec4 fragPosLS) {
    vec3 projCoords = fragPosLS.xyz / fragPosLS.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r; 

    float currentDepth = projCoords.z;

    float bias = 0.001;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    if(projCoords.z > 1.0) shadow = 0.0;

    return shadow;
}


void main()
{
    // --- 1. Base Color ---
    vec3 grass = texture(grassTex, v_texCoord).rgb;
    vec3 rock  = texture(rockTex, v_texCoord).rgb;
    vec3  snow = texture(snowTex, v_texCoord).rgb;

    vec3 baseColor;
    
    // Simple blending logic
    if (Height < 5.0) {
        baseColor = grass;
    } else if (Height < 20.0) {
        // Blend grass to rock between height 5 and 20
        float t = clamp((Height - 0) / 15.0, 0.0, 1.0);
        baseColor = mix(grass, rock, t);
    } else {
        // Blend rock to snow between height 20 and 35
        float t = clamp((Height - 15.0) / 15.0, 0.0, 1.0);
        baseColor = mix(rock, snow, t);
    }

    vec3 norm = normalize(v_normal);

    // --- 2. Lighting ---
    vec3 totalLighting = vec3(0.0);
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

        vec3 viewDir = normalize(cameraPos - v_fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), max(lightProperties[i].w, 1.0));
        vec3 specular = lightProperties[i].z * spec * lightColor;

        vec3 att = lightAttenuations[i].xyz;
        float attenuation = 1.0 / (att.x + att.y * dist + att.z * dist * dist);

        if (i == 0) {
            totalLighting += (ambient + (1.0 - shadow) * (diffuse + specular)) * attenuation;
        } else {
            totalLighting += (ambient + diffuse + specular) * attenuation;
        }
    }
    
    // float dist = length(v_fragPos - cameraPos.xyz);

//     float heightFactor = clamp((fogMaxHeight - v_fragPos.y) / (fogMaxHeight - fogMinHeight), 0.0, 1.0);
    
//     float effectiveDensity = fogDensity * heightFactor;
    
//     float mistFactor = exp(-effectiveDensity * dist);
//     mistFactor = clamp(mistFactor, 0.0, 1.0);

//  vec3 outColor = mix(mistColor, result, mistFactor);

    FragColor = vec4(totalLighting * baseColor, 1.0);

}