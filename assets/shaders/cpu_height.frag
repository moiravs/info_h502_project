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
    if (h > 0.8) baseColor = vec3(1.0, 1.0, 1.0);

    // --- 2. Lighting Logic ---
    vec3 sum = vec3(0, 0, 0);
    for (int i = 0; i < count; i++) {
        // Ambient
        vec3 ambient = properties[i].x * vec3(1.0);

        // Diffuse
        vec3 norm = normalize(v_normal);
        vec3 lightDir = normalize(vec3(positions[i]) - v_fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * properties[i].y * vec3(1.0);

        sum += ambient + diffuse;
    }

    // --- 3. Final Composition ---
    vec3 finalResult = sum * baseColor;
    FragColor = vec4(finalResult, 1.0);
}
