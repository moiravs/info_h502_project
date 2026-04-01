#version 330 core
out vec4 FragColor;

in vec4 clipSpace;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

uniform float time;

void main() {

    vec2 ndc = (clipSpace.xy / clipSpace.w)/2.0 + 0.5;
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoords = vec2(ndc.x, 1.0-ndc.y);
    vec4 reflectColour = texture(reflectionTexture, reflectTexCoords);
    vec4 refractColour = texture(refractionTexture, refractTexCoords);
    // Define water colors

    FragColor = mix(reflectColour, refractColour, 0.5); 
}
