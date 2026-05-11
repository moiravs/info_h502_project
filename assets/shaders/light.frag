#version 410 core

layout(location = 0) out vec4 lColor;

#define MAX_LIGHTS 128

layout(std140) uniform Lights {
    vec4 lightPositions[MAX_LIGHTS];
    vec4 lightProperties[MAX_LIGHTS];
    vec4 lightAttenuations[MAX_LIGHTS];
    vec4 lightColors[MAX_LIGHTS];

    mat4 sunPV;

    int lightCount;
    int pad1;
    int pad2;
    int pad3;
};

layout(std140) uniform CameraInfo {
    mat4 projection;
    mat4 view;
    vec4 camPosition;
    vec4 camRight;
    vec4 camUp;
};

in vec2 TexCoords;

uniform sampler2D color;
uniform sampler2D normal;
uniform sampler2D depth;
uniform sampler2D shadow;

vec3 getWorldPos(vec2 uv, float depthValue)
{
    // convert [0,1] coordinates into [-1,1]^3 clip space coordinates
    vec4 clip = vec4(uv * 2.0 - 1.0, depthValue * 2.0 - 1.0, 1.0);

    // clip = PV * WorldPos, then, just solve for WorldPos
    vec4 viewPos = inverse(projection) * clip;
    viewPos /= viewPos.w;

    vec4 worldPos = inverse(view) * viewPos;
    return worldPos.xyz;
}

vec3 shade(vec3 worldPos, vec3 lighting)
{
    vec4 sunSpacePos = sunPV * vec4(worldPos, 1);

    vec3 projCoords = sunSpacePos.xyz / sunSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadow, projCoords.xy).r;
    float currentDepth = projCoords.z;

    if (closestDepth < currentDepth - 0.01) {
        return vec3(0, 0, 0);
    }

    return lighting;
}

void main()
{
    float depth = texture(depth, TexCoords).r;
    vec3 worldPos = getWorldPos(TexCoords, depth);
    vec4 baseColor = texture(color, TexCoords);

    if (depth >= 1.0) {
        lColor = baseColor;
        return;
    }

    vec3 norm = texture(normal, TexCoords).xyz;

    vec3 viewDir = normalize(camPosition.xyz - worldPos);

    vec3 totalLighting = vec3(0.0);

    for (int i = 0; i < lightCount; i++)
    {
        vec3 lightPos = lightPositions[i].xyz;

        vec3 lightColor = lightColors[i].xyz;

        vec3 ambient = lightProperties[i].x * lightColor;

        vec3 toLight = lightPos - worldPos;
        float dist = length(toLight);
        vec3 lightDir = normalize(toLight);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightProperties[i].y * lightColor;

        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(
            max(dot(viewDir, reflectDir), 0.0),
            lightProperties[i].w
        );

        vec3 specular = lightProperties[i].z * spec * lightColor;

        vec3 att = lightAttenuations[i].xyz;
        float attenuation = 1.0 /
        (att.x + att.y * dist + att.z * dist * dist);

        vec3 contribution = ((ambient + diffuse + specular) * attenuation) * exp(-max(0, -lightPos.y/100));
        if (i == 0)
            totalLighting += shade(worldPos, contribution);
        else
            totalLighting += contribution;
    }

    vec3 shadedLighting = shade(worldPos, totalLighting);

    lColor = vec4(baseColor.rgb * shadedLighting, baseColor.a);
}