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
const float atmosphereMinHeight = -15.0;
const float densityFalloff = 25;
uniform float atmosphereIntensity = 1.5; 

const vec3 betaRayleigh = vec3(0.0058, 0.0135, 0.0331); 
const vec3 betaMie = vec3(0.004, 0.004, 0.004);

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
    if (rayOrigin.y < atmosphereMinHeight) return vec2(0, 0);
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

float densityAtPoint(vec3 pos) {
    float heightAboveSurface = max(0.0, pos.y - atmosphereMinHeight);
    float height01 = heightAboveSurface / (atmosphereMaxHeight - atmosphereMinHeight);
    return exp(-height01 * densityFalloff) * (1.0 - height01);
}

float calculateOpticalDepth(vec3 rayOrigin, vec3 rayDir, float rayLength) {
    vec3 samplePoint = rayOrigin;
    float stepSize = rayLength / (float(numOpticalDepthPoints) - 1);
    float opticalDepth = 0.0;

    for (int i = 0; i < numOpticalDepthPoints; i++) {
        opticalDepth += densityAtPoint(samplePoint) * stepSize;
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

vec3 calculateLight(vec3 rayOrigin, vec3 rayDir, float rayLength) {
    vec3 inScatterPoint = rayOrigin;
    vec3 dirToSun = normalize(vec3(sunDir));
    if (dirToSun.y < 0) return vec3(0);
    float stepSize = rayLength / (float(numInScatteringPoints) - 1);
    float inScatteredLight = 0;

    for (int i = 0; i < numInScatteringPoints; i++) {
        float localDensity = densityAtPoint(inScatterPoint);
        float sunRayLength = rayAtm(inScatterPoint, dirToSun).y;

        // calculate optical depth from the point to the sun
        float sunRayOpticalDepth = calculateOpticalDepth(inScatterPoint, dirToSun, sunRayLength);
        // calculate optical depth from the point to the camera
        float viewRayOpticalDepth = calculateOpticalDepth(inScatterPoint, -rayDir, i * stepSize);

        //vec3 totalOpticalDepth = (betaRayleigh + betaMie) * (sunRayOpticalDepth);
        float transmittance = exp(-(sunRayOpticalDepth + viewRayOpticalDepth));
//        float transmittance = exp(-(viewRayOpticalDepth));

//        vec3 attenuation = localDensity * transmittance * stepSize;
        inScatteredLight += localDensity * transmittance * stepSize;
//        inScatteredLight += attenuation * (betaRayleigh * pRayleigh + betaMie * pMie);

        inScatterPoint += rayDir * stepSize;
    }

    return vec3(inScatteredLight);

//    scatteredLight *= atmosphereIntensity;
//
//    vec3 sunDisc = vec3(0.0);
//
//    const float strictSunSize = 0.9995;
//    const float strictSunEdge = 0.9998;
//
//    if (cosAngle > strictSunSize && dirToSun.y > 0.0) {
//        if (maxDistance >= 90000.0) {
//            float sunAlpha = smoothstep(strictSunSize, strictSunEdge, cosAngle);
//
//            vec3 sunColor = mix(vec3(5.0, 1.2, 0.1), vec3(4.0, 3.8, 3.5), clamp(dirToSun.y * 6.0, 0.0, 1.0));
//            float horizonGlowFade = clamp(dirToSun.y * 8.0, 0.0, 1.0);
//            sunDisc = sunColor * sunAlpha * horizonGlowFade;
//        }
//    }
//
//    float fogFactor = 0;
//    vec3 finalSceneColor = mix(originalCol, scatteredLight, fogFactor);
//
//    return finalSceneColor + scatteredLight * 0.2 + sunDisc;
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
        vec3 light = calculateLight(pointInAtmosphere, rayDir, distInAtm - 2 * eps);
        lColor = vec4(originalCol * (1 - light.x) + light.x, 1);
    } else {
        lColor = vec4(originalCol, 1);
    }
}