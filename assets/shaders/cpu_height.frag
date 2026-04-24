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

in float Height;
in vec3 v_normal;
in vec3 v_fragPos;

void main()
{
    // --- 1. Base Color ---
    float h = (Height + 16.0) / 32.0;
    h = clamp(h, 0.0, 1.0);

    vec3 darkGreen = vec3(0.05, 0.2, 0.05);
    vec3 lightGreen = vec3(0.3, 0.8, 0.3);
    vec3 baseColor = mix(darkGreen, lightGreen, h);

    if (h > 0.8)
    baseColor = vec3(1.0);

    vec3 norm = normalize(v_normal);

    // --- 2. Lighting ---
    vec3 result = vec3(0.0);

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
        float attenuation =
        1.0 / (att.x + att.y * dist + att.z * dist * dist);

        result += (ambient + diffuse) * baseColor * attenuation;
    }

    FragColor = vec4(result, 1.0);
}