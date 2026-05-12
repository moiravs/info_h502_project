#version 410 core
layout(location = 0) out vec4 gColor;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gMaterial;

in vec2 v_t; 
in vec3 v_normal;
in vec3 v_fragPos;

uniform sampler2D texture0;
uniform vec2 material;

void main() {
    vec4 texColor = texture(texture0, v_t);
    if(texColor.a < 0.1) {
        discard;
    }

    vec3 norm = normalize(v_normal);

    gColor = texColor.rgba;
    gNormal = norm;
    gMaterial = vec4(material, 0, 0);
}