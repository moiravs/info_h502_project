#version 410 core

layout(location = 0) out vec4 lColor;

in vec2 TexCoords; 

uniform sampler2D color;
uniform sampler2D depth;
uniform vec2 viewport_size;

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

layout(std140) uniform CameraInfo {
    mat4 projection;
    mat4 iProj;
    mat4 view;
    mat4 iView;
    vec4 camPosition;
    vec4 camRight;
    vec4 camUp;
};

const int numInScatteringPoints = 12;
const int numOpticalDepthPoints = 4;

const float atmosphereRadius = 600.0;    
const float densityFalloff = 3.0;  
uniform float atmosphereIntensity = 1.5; 

const vec3 betaRayleigh = vec3(0.0058, 0.0135, 0.0331); 
const vec3 betaMie = vec3(0.004, 0.004, 0.004);

float rayDomeIntersection(vec3 rayDir, float radius) {
    return radius; 
}

float densityAtPoint(vec3 pos) {
    float height01 = clamp(max(0.0, pos.y) / atmosphereRadius, 0.0, 1.0);
    return exp(-height01 * densityFalloff) * (1.0 - height01);
}

float calculateOpticalDepth(vec3 rayOrigin, vec3 rayDir, float rayLength) {
    vec3 samplePoint = rayOrigin;
    float stepSize = rayLength / float(numOpticalDepthPoints);
    float opticalDepth = 0.0;

    for (int i = 0; i < numOpticalDepthPoints; i++) {
        opticalDepth += densityAtPoint(samplePoint - camPosition.xyz) * stepSize;
        samplePoint += rayDir * stepSize;
    }
    return opticalDepth;
}

float rayleighPhase(float cosAngle) {
    return 3.0 / (16.0 * 3.141592) * (1.0 + cosAngle * cosAngle);
}

float miePhase(float cosAngle, float g) {
    float g2 = g * g;
    return 1.0 / (4.0 * 3.141592) * (1.0 - g2) / pow(1.0 + g2 - 2.0 * g * cosAngle, 1.5);
}

vec3 calculateAtmosphere(vec3 rayOrigin, vec3 rayDir, float maxDistance, vec3 originalCol) {
    vec3 dirToSun = normalize(sunDir.xyz);
    float cosAngle = dot(rayDir, dirToSun);
    
    float pRayleigh = rayleighPhase(cosAngle);
    float pMie = miePhase(cosAngle, 0.75);

    float distanceToSpace = rayDomeIntersection(rayDir, atmosphereRadius);
    float marchLength = min(maxDistance, distanceToSpace);
    
    float stepSize = marchLength / float(numInScatteringPoints);
    vec3 currentPos = rayOrigin;

    vec3 scatteredLight = vec3(0.0);
    float viewRayOpticalDepth = 0.0;

    for (int i = 0; i < numInScatteringPoints; i++) {
        vec3 localPos = currentPos - rayOrigin;
        float localDensity = densityAtPoint(localPos);
        
        viewRayOpticalDepth += localDensity * stepSize;

        float sunRayOpticalDepth = calculateOpticalDepth(currentPos, dirToSun, atmosphereRadius * 0.5);

        vec3 totalOpticalDepth = (betaRayleigh + betaMie) * (viewRayOpticalDepth + sunRayOpticalDepth);
        vec3 transmittance = exp(-totalOpticalDepth * 0.1);

        vec3 attenuation = localDensity * transmittance * stepSize;
        scatteredLight += attenuation * (betaRayleigh * pRayleigh + betaMie * pMie);

        currentPos += rayDir * stepSize;
    }

    scatteredLight *= atmosphereIntensity;

    vec3 sunDisc = vec3(0.0);
    
    const float strictSunSize = 0.9995; 
    const float strictSunEdge = 0.9998;

    if (cosAngle > strictSunSize && dirToSun.y > 0.0) {
        if (maxDistance >= 90000.0) { 
            float sunAlpha = smoothstep(strictSunSize, strictSunEdge, cosAngle);
            
            vec3 sunColor = mix(vec3(5.0, 1.2, 0.1), vec3(4.0, 3.8, 3.5), clamp(dirToSun.y * 6.0, 0.0, 1.0));
            float horizonGlowFade = clamp(dirToSun.y * 8.0, 0.0, 1.0);
            sunDisc = sunColor * sunAlpha * horizonGlowFade;
        }
    }

    float fogFactor = 0;
    vec3 finalSceneColor = mix(originalCol, scatteredLight, fogFactor);

    return finalSceneColor + scatteredLight * 0.2 + sunDisc;
}

void main() {
    vec3 originalCol = texture(color, TexCoords).rgb;
    float rawDepth = texture(depth, TexCoords).r;
    
    vec2 ndc = TexCoords * 2.0 - 1.0;
    float zNDC = rawDepth * 2.0 - 1.0; 
    
    vec4 clipPos = vec4(ndc, zNDC, 1.0);
    vec4 viewPos = iProj * clipPos;
    viewPos /= viewPos.w;
    
    vec3 rayDir = normalize((iView * vec4(normalize(viewPos.xyz), 0.0)).xyz);
    float sceneDepth = length(viewPos.xyz);
    
    if (rawDepth >= 0.9999) {
        sceneDepth = 999999.0;
    }
    
    vec3 rayOrigin = camPosition.xyz;
    vec3 finalColor = calculateAtmosphere(rayOrigin, rayDir, sceneDepth, originalCol);
    
    if (any(isnan(finalColor)) || any(isinf(finalColor))) {
        lColor = vec4(originalCol, 1.0);
    } else {
        lColor = vec4(finalColor, 1.0);
    }
}