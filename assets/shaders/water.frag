#version 330 core
out vec4 FragColor;

in vec4 clipSpace;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;

in vec2 TexCoord;


uniform float time;

void main() {

    vec2 ndc = (clipSpace.xy / clipSpace.w)/2.0 + 0.5;
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoords = vec2(ndc.x, 1.0-ndc.y);


    vec2 distortion1 = texture(dudvMap, vec2(TexCoord.x, TexCoord.y)).rg * 2.0 - 1.0;

    reflectTexCoords += distortion1;
    refractTexCoords += distortion1;

    vec4 reflectColour = texture(reflectionTexture, reflectTexCoords);
    vec4 refractColour = texture(refractionTexture, refractTexCoords);
    // Define water colors

    FragColor = mix(reflectColour, refractColour, 0.5); 
}
