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

layout(std140) uniform CameraInfo {
    mat4 projection;
    mat4 view;
    vec4 camPosition;
    vec4 camRight;
    vec4 camUp;
};

in vec2 v_t; 
in vec3 v_normal;
in vec3 v_fragPos;

uniform sampler2D texture0;

void main() {

    vec4 texColor = texture(texture0, v_t);
    if(texColor.a < 0.1) {
        discard;
    }

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

        result += (ambient + diffuse) * texColor.rgb * attenuation;

    }

   FragColor = vec4(texColor.rgb,  texColor.a);
}
