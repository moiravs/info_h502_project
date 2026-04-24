#version 410 core
out vec4 FragColor;

#define MAX_LIGHTS 128
layout(std140) uniform Lights {
    vec4 positions[MAX_LIGHTS];
    vec4 properties[MAX_LIGHTS];
    vec4 attenuations[MAX_LIGHTS];
    int count;
    int pad1;
    int pad2;
    int pad3;
};

in float Height;
in vec3 v_normal;
in vec3 v_fragPos;

void main()
{
    // --- 1. Base Color Logic (Your existing code) ---
    float h = (Height + 16.0) / 32.0;
    h = clamp(h, 0.0, 1.0);

    vec3 darkGreen = vec3(0.05, 0.2, 0.05);
    vec3 lightGreen = vec3(0.3, 0.8, 0.3);
    vec3 baseColor = mix(darkGreen, lightGreen, h);
    vec3 norm = normalize(v_normal);
    if (h > 0.8) baseColor = vec3(1.0, 1.0, 1.0);

    // --- 2. Lighting Logic ---
    vec3 lights = vec3(0,0,0);

    for (int i = 0; i < count; i++) {
        // Ambient
        vec3 ambient = properties[i].x * vec3(1.0);

        // Diffuse
        vec3 lightDir = normalize(positions[i].xyz - v_fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        float dist = length(positions[i].xyz - v_fragPos);
        vec3 diffuse = diff * properties[i].y * vec3(1.0);
        float attenuation = 1 / (attenuations[i].x + attenuations[i].y * dist + attenuations[i].z * dist * dist);

        // --- 3. Final Composition ---
        lights += (ambient + diffuse) * baseColor * attenuation;
    }

    FragColor = vec4(lights, 1.0);
}
