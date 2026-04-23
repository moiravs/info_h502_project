#version 410 core
out vec4 FragColor;

struct Light {
    vec3 light_pos;
    float ambient_strength;
    float diffuse_strength;
    float specular_strength;
};

in vec2 v_t; 
in vec3 v_normal;
in vec3 v_fragPos;

uniform sampler2D texture0;
uniform Light light;
uniform vec3 u_view_pos; 

void main() { 

    vec3 ambient = light.ambient_strength * vec3(1.0); 
    
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(light.light_pos - v_fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse_strength * vec3(1.0);
    
    vec4 texColor = texture(texture0, v_t);
    if(texColor.a < 0.1) {
        discard;
    }

    vec3 result = (ambient + diffuse) * texColor.rgb;
    FragColor = vec4(result, texColor.a);
} 
