#version 410 core
out vec4 FragColor;

in vec4 clipSpace;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;

const float waveStrength = 0.005f;
in vec2 TexCoord;

uniform float moveFactor;


uniform float time;

void main() {

    vec2 ndc = (clipSpace.xy / clipSpace.w)/2.0 + 0.5;
    vec2 refractTexCoords = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoords = vec2(ndc.x, 1.0-ndc.y);


    vec2 distortion1 = (texture(dudvMap, vec2(TexCoord.x + moveFactor, TexCoord.y)).rg * 2.0 - 1.0) * waveStrength;
    vec2 distortion2 = (texture(dudvMap, vec2(-TexCoord.x + moveFactor, TexCoord.y + moveFactor)).rg * 2.0 - 1.0) * waveStrength;

    vec2 totalDistortion = distortion1 + distortion2;


    reflectTexCoords += totalDistortion;
    refractTexCoords += totalDistortion;

    // refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);
    // reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);
    // reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);

    vec4 reflectColour = texture(reflectionTexture, reflectTexCoords);
    vec4 refractColour = texture(refractionTexture, refractTexCoords);
    // Define water colors

    FragColor = mix(reflectColour, refractColour, 0.5); 
 //FragColor = texture(dudvMap, TexCoord); 
}
