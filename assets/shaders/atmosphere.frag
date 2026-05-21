#version 410 core

// this whole shader is heavily inspired by https://www.youtube.com/watch?v=DxfEbulyFcY

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
const int numOpticalDepthPoints = 10;
const float FLOAT_MAX = 10e20;

const float atmosphereMaxHeight = 300.0;
const float atmosphereMieMaxHeight = 300.0;
const float atmosphereMinHeight = -15.0;
const float densityFalloff = 25;
const float atmosphereIntensity = 0.7;

const float scatteringStrength = 1;
const float scatterR = pow(400.0 / 700.0, 4) * scatteringStrength;
const float scatterG = pow(400.0 / 530.0, 4) * scatteringStrength;
const float scatterB = pow(400.0 / 440.0, 4) * scatteringStrength;
const vec3 betaRayleigh = vec3(scatterR, scatterG, scatterB);
//const vec3 betaRayleigh = vec3(5.8e-6, 13.5e-6, 33.1e-6);
const vec3 betaMie = vec3(0.1);

vec3 getWorldPos(vec2 uv, float depthValue)
{
    // convert [0,1] coordinates into [-1,1]^3 clip space coordinates
    vec4 clip = vec4(uv * 2.0 - 1.0, depthValue * 2.0 - 1.0, 1.0);

    // clip = PV * WorldPos, then, just solve for WorldPos
    vec4 viewPos = iProj * clip;
    viewPos /= viewPos.w;

    vec4 worldPos = iView * viewPos;
    return worldPos.xyz;
}

// return the couple (distToAtm, distInAtm)
vec2 rayAtm(vec3 rayOrigin, vec3 rayDir) {
    if (rayOrigin.y < atmosphereMaxHeight) {
        // we are already in
        if (abs(rayDir.y) < 0.001) return vec2(0, FLOAT_MAX); // if the ray is flat, we stay in

        // since we are in, we can stop the ray by either encountering the ground or the atmosphere limit
        float t_atm = (atmosphereMaxHeight - rayOrigin.y) / rayDir.y;
        if (t_atm > 0) return vec2(0, t_atm); // we are were looking at the sky!

        float t_ground = (atmosphereMinHeight - rayOrigin.y) / rayDir.y;
        return vec2(0, t_ground);
    } else {
        // we are out
        if (abs(rayDir.y) < 0.001) return vec2(FLOAT_MAX, 0); // if the ray is flat, we stay out

        float t_atm = (atmosphereMaxHeight - rayOrigin.y) / rayDir.y;
        if (t_atm < 0) return vec2(FLOAT_MAX, 0); // we are looking in the wrong direction, we stay out

        // we know that we will encounter the ground at some point.
        float t_ground = (atmosphereMinHeight - rayOrigin.y) / rayDir.y;
        return vec2(t_atm, t_ground - t_atm);
    }
}

float densityAtPoint(vec3 pos, float maxHeight) {
    float heightAboveSurface = max(0.0, pos.y - atmosphereMinHeight);
    float height01 = heightAboveSurface / (maxHeight - atmosphereMinHeight);
    return exp(-height01 * densityFalloff) * (1.0 - height01);
}

vec2 calculateOpticalDepth(vec3 rayOrigin, vec3 rayDir, float rayLength) {
    vec3 samplePoint = rayOrigin;
    float stepSize = rayLength / (float(numOpticalDepthPoints) - 1);
    float opticalDepthRayleigh = 0.0;
    float opticalDepthMie = 0.0;

    for (int i = 0; i < numOpticalDepthPoints; i++) {
        opticalDepthRayleigh += densityAtPoint(samplePoint, atmosphereMaxHeight) * stepSize;
        opticalDepthMie += densityAtPoint(samplePoint, atmosphereMieMaxHeight) * stepSize;
        samplePoint += rayDir * stepSize;
    }
    return vec2(opticalDepthRayleigh, opticalDepthMie);
}

float rayleighPhase(float cosAngle) {
    return (1.0 + cosAngle * cosAngle) / 2;
}

float miePhase(float cosAngle, float g) {
    float g2 = g * g;
    return 1.0 / (4.0 * 3.141592) * (1.0 - g2) / pow(1.0 + g2 - 2.0 * g * cosAngle, 1.5);
}

vec3 calculateLight(vec3 rayOrigin, vec3 rayDir, float rayLength, vec3 originalColor) {
    vec3 inScatterPoint = rayOrigin;
    vec3 dirToSun = normalize(vec3(sunDir));

    float stepSize = rayLength / (float(numInScatteringPoints) - 1);
    vec3 inScatteredLight = vec3(0);
    vec2 viewRayOpticalDepth = vec2(0);
    float angle = dot(dirToSun, rayDir);
    vec3 mie = vec3(0);

    for (int i = 0; i < numInScatteringPoints; i++) {
        float localRayleighDensity = densityAtPoint(inScatterPoint, atmosphereMaxHeight);
        float localMieDensity = densityAtPoint(inScatterPoint, atmosphereMieMaxHeight);
        float sunRayLength = rayAtm(inScatterPoint, dirToSun).y;

        // calculate optical depth from the point to the sun
        vec2 sunRayOpticalDepth = calculateOpticalDepth(inScatterPoint, dirToSun, sunRayLength);
        // calculate optical depth from the point to the camera
        viewRayOpticalDepth = calculateOpticalDepth(inScatterPoint, -rayDir, i * stepSize);

        vec3 transmittance = exp(-((sunRayOpticalDepth.x + viewRayOpticalDepth.x) * betaRayleigh +
                                  (sunRayOpticalDepth.y + viewRayOpticalDepth.y) * betaMie));

        vec3 scatterRayleigh = localRayleighDensity * betaRayleigh * rayleighPhase(angle);

        vec3 scatterMie = localMieDensity * betaMie * miePhase(angle, 0.76);

        inScatteredLight += (scatterRayleigh + scatterMie) * transmittance * stepSize;

        inScatterPoint += rayDir * stepSize;
    }

    return originalColor + inScatteredLight * atmosphereIntensity;
}

void main() {
    vec3 originalCol = texture(color, TexCoords).rgb;
    vec3 camPos = vec3(camPosition);
    float rawDepth = texture(depth, TexCoords).r;
    vec3 worldPos = getWorldPos(TexCoords, rawDepth);

    vec3 ray = worldPos - camPos;
    vec3 rayDir = normalize(ray);
    float sceneDepth = length(ray);
    
    vec3 rayOrigin = camPosition.xyz;
    vec2 hitInfo = rayAtm(rayOrigin, rayDir);
    float distToAtm = hitInfo.x;
    float distInAtm = min(hitInfo.y, sceneDepth - distToAtm);

    if (distInAtm > 0.0) {
        const float eps = 0.0001;
        vec3 pointInAtmosphere = rayOrigin + rayDir * (distToAtm + eps);
        vec3 light = calculateLight(pointInAtmosphere, rayDir, distInAtm - 2 * eps, originalCol);
        lColor = vec4(light, 1);
    } else {
        lColor = vec4(originalCol, 1);
    }
}