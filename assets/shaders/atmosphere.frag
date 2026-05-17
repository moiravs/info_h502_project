#version 410 core

layout(location = 0) out vec4 lColor;

in vec2 TexCoords; 

uniform sampler2D color;
uniform sampler2D normal;
uniform sampler2D depth;
uniform sampler2D material;
uniform sampler2D shadow;
uniform vec2 viewport_size;
uniform vec3 sunDirection;

layout(std140) uniform Lights {
    vec4 lightPositions[32];
    vec4 lightProperties[32];   
    vec4 lightAttenuations[32]; 
    vec4 lightColors[32];
    mat4 sunPV;
    vec3 sunDir;
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

void GADD(vec3 PP, vec3 sunDirection, float density, float falloff, out float g, out vec3 li) {
    g = density * exp(-falloff * max(0.0, PP.y));
    
    float NdotL = max(dot(vec3(0.0, 1.0, 0.0), normalize(sunDirection)), 0.0); 
    vec3 sunColor = vec3(1.0, 0.95, 0.85);
    
    vec3 ambientSky = vec3(0.2, 0.35, 0.5) * 0.4;
    li = (sunColor * NdotL * 4.0) + ambientSky; 
}

void main() {
    float rawDepth = texture(depth, TexCoords).r;
    bool isSky = (rawDepth >= 1.0);
    
    vec3 rayDir = get_ray_direction();
    vec3 rawSceneColor = texture(color, TexCoords).rgb; 

    float density = 0.00007;    
    float falloff = 0.002;     
    float integstart = 0.0;
    float integend    = 4000.0; 
    float minstepsize = 4.0;   
    float maxstepsize = 150.0;    
    float k = 12.0;           

    float maxDist     = isSky ? integend : min(linearize(rawDepth), integend);
    float te = maxDist - 0.0001;

    vec3 origin = camPosition.xyz;
    
    float t = integstart;
    float dtau = 0.0;
    vec3 li = vec3(0.0);
    
    vec3 PP = origin + t * rayDir;
    GADD(PP, sunDirection, density, falloff, dtau, li);
    
    float ss = min(clamp(1.0 / (k * dtau + 0.001), minstepsize, maxstepsize), te - t);
    t += ss;
    
    vec3 Cv = vec3(0.0); 
    vec3 Ov = vec3(0.0); 
    
    vec3 activesunDirection = normalize(sunDirection);
    float cosTheta = dot(rayDir, activesunDirection);
    float rayleighPhase = 0.75 * (1.0 + cosTheta * cosTheta);
    
    while (t <= te) {
        float last_dtau = dtau;
        vec3 last_li = li;
        
        PP = origin + t * rayDir;
        GADD(PP, sunDirection, density, falloff, dtau, li);
        
        float tau = 0.5 * ss * (dtau + last_dtau);
        vec3 lighttau = 0.5 * ss * (li * dtau + last_li * last_dtau);
        
        // --- FIXED: Re-balanced wavelength curves ---
        // Brought Red and Green closer to Blue to prevent the dark "neon blue" drowning effect.
        vec3 dO = vec3(1.0) - vec3(exp(-tau * 0.7), exp(-tau * 1.3), exp(-tau * 3.5));
        vec3 dC = lighttau * dO * rayleighPhase;
        
        Cv += (vec3(1.0) - Ov) * dC;
        Ov += (vec3(1.0) - Ov) * dO;
        
        ss = min(clamp(1.0 / (k * dtau + 0.001), minstepsize, maxstepsize), te - t);
        ss = max(ss, 0.005);
        t += ss;
        
        if(ss <= 0.005 && t < te) { t += minstepsize; }
    }
    
    // --- FIXED: Multiplied color up to shine against your black background color ---
    vec3 finalOutput = 45.0 * Cv + (vec3(1.0) - Ov) * rawSceneColor;

    // Sun Disk Overlay (Paints a crisp, bright sun directly in the sky)
    if (isSky) {
        float sunDisk = smoothstep(0.9980, 0.9997, cosTheta);
        finalOutput += vec3(35.0, 32.0, 27.0) * sunDisk * (vec3(1.0) - Ov);
    }

    // HDR Tonemapping & Gamma processing
    finalOutput = finalOutput / (finalOutput + vec3(1.0));
    finalOutput = pow(finalOutput, vec3(1.0 / 2.2)); 

    lColor = vec4(finalOutput, 1.0);
}