#version 410 core
out vec4 FragColor;

struct Light {
    vec3 light_pos;
    float ambient_strength;
    float diffuse_strength;
    float specular_strength;
};

in float Height;
in vec3 v_normal;
in vec3 v_fragPos;

uniform Light light;

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
    // Ambient
    vec3 ambient = light.ambient_strength * vec3(1.0);

    // Diffuse
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(light.light_pos - v_fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse_strength * vec3(1.0);

    // --- 3. Final Composition ---
    vec3 finalResult = (ambient + diffuse) * baseColor;
    FragColor = vec4(finalResult, 1.0);
}
