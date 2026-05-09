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


uniform float textureScale = 0.1; // Adjust this to tile your textures

void main()
{
    // --- 1. Prepare Triplanar Weights ---
    vec3 norm = normalize(v_normal);
    vec3 blending = abs(norm);
    blending = pow(blending, vec3(8.0)); // Sharpen the transition
    float totalWeight = blending.x + blending.y + blending.z;
    blending /= totalWeight;

    // --- 2. Sample Textures Triplanarly ---
    // Function-like approach: sample each texture from 3 sides
    vec3 grass = (texture(grassTex, v_fragPos.zy * textureScale).rgb * blending.x +
                  texture(grassTex, v_fragPos.xz * textureScale).rgb * blending.y +
                  texture(grassTex, v_fragPos.xy * textureScale).rgb * blending.z);

    vec3 rock  = (texture(rockTex, v_fragPos.zy * textureScale).rgb * blending.x +
                  texture(rockTex, v_fragPos.xz * textureScale).rgb * blending.y +
                  texture(rockTex, v_fragPos.xy * textureScale).rgb * blending.z);

    vec3 snow  = (texture(snowTex, v_fragPos.zy * textureScale).rgb * blending.x +
                  texture(snowTex, v_fragPos.xz * textureScale).rgb * blending.y +
                  texture(snowTex, v_fragPos.xy * textureScale).rgb * blending.z);

    // --- 3. Base Color & Slope Blending ---
    vec3 baseColor;
    
    // Logic: If slope is steep (norm.y is low), force Rock texture
    float slope = clamp(norm.y, 0.0, 1.0);
    float rockSplat = 1.0 - smoothstep(0.4, 0.7, slope); // 0.7 = flat, 0.4 = steep

    if (Height < 5.0) {
        baseColor = grass;
    } else if (Height < 20.0) {
        float t = clamp((Height - 5.0) / 15.0, 0.0, 1.0);
        baseColor = mix(grass, rock, t);
    } else {
        float t = clamp((Height - 20.0) / 15.0, 0.0, 1.0);
        baseColor = mix(rock, snow, t);
    }

    // Apply the slope-based rock override to fix stretching on cliffs
    baseColor = mix(baseColor, rock, rockSplat);

    // --- 4. Lighting (Unchanged Logic) ---
    vec3 totalLighting = vec3(0.0);
    //float shadow = ShadowCalculation(FragPosLightSpace);

    for (int i = 0; i < lightCount; i++)
    {
        // ... (Keep your existing lighting loop here)
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
            totalLighting += (ambient + (1.0) * (diffuse + specular)) * attenuation;
        } else {
            totalLighting += (ambient + diffuse + specular) * attenuation;
        }
    }
    
    FragColor = vec4(totalLighting * baseColor, 1.0);
}

