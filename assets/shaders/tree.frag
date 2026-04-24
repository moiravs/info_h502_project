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

layout(std140) uniform CameraInfo {
    mat4 projection;
    mat4 view;
    vec4 camPosition;
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

    vec3 col = vec3(0, 0, 0);
    vec3 norm = normalize(v_normal);

    for (int i = 0; i < count; i++) {
        vec3 ambient = properties[i].x * vec3(1.0); // Assuming white light

        // Diffuse
        vec3 lightDir = normalize(positions[i].xyz - v_fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * properties[i].y * vec3(1.0);

        // Alpha Discard (Crucial for leaves)
        col += (ambient + diffuse);
    }
    FragColor = vec4(col * texColor.rgb, texColor.a);
}
