#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform float time;

void main() {
    // Define water colors
    vec3 deepBlue = vec3(0.0, 0.2, 0.5);
    vec3 shallowBlue = vec3(0.1, 0.4, 0.8);

    // Create a shimmering effect using time and texture coordinates
    float shimmer = sin(TexCoord.x * 10.0 + time) * 0.5 + 0.5;
    vec3 waterColor = mix(deepBlue, shallowBlue, shimmer);

    // Set alpha (0.6 is semi-transparent)
    FragColor = vec4(waterColor, 0.6); 
}
