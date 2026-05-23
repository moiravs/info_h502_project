#version 410 core

layout(location = 0) out vec4 gColor;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gMaterial;

in float Height;
in vec3 v_normal;
in vec3 v_fragPos;

in vec2 v_texCoord;
uniform sampler2D grassTex;
uniform sampler2D rockTex;
uniform sampler2D snowTex;

uniform vec2 material;
uniform float textureScale = 0.1;

void main()
{
    vec3 norm = normalize(v_normal);
    vec3 blending = abs(norm);
    blending = pow(blending, vec3(8.0)); // Sharpen the transition
    float totalWeight = blending.x + blending.y + blending.z;
    blending /= totalWeight;

    vec3 grass = (texture(grassTex, v_fragPos.zy * textureScale).rgb * blending.x +
                  texture(grassTex, v_fragPos.xz * textureScale).rgb * blending.y +
                  texture(grassTex, v_fragPos.xy * textureScale).rgb * blending.z);

    vec3 rock  = (texture(rockTex, v_fragPos.zy * textureScale).rgb * blending.x +
                  texture(rockTex, v_fragPos.xz * textureScale).rgb * blending.y +
                  texture(rockTex, v_fragPos.xy * textureScale).rgb * blending.z);

    vec3 snow  = (texture(snowTex, v_fragPos.zy * textureScale).rgb * blending.x +
                  texture(snowTex, v_fragPos.xz * textureScale).rgb * blending.y +
                  texture(snowTex, v_fragPos.xy * textureScale).rgb * blending.z);

    vec3 baseColor;
    
    float slope = clamp(norm.y, 0.0, 1.0);
    float rockSplat = 1.0 - smoothstep(0.4, 0.7, slope); // 0.7 = flat, 0.4 = steep

    if (Height < 5.0) {
        baseColor = grass;
    } else if (Height < 20.0) {
        float t = clamp((Height - 5.0) / 15.0, 0.0, 1.0);
        baseColor = mix(grass, rock, t);
    } else {
        float t = clamp((Height - 20.0) / 15.0, 0.0, 1.0);
        baseColor = mix(rock, snow, t);
    }

    baseColor = mix(baseColor, rock, rockSplat);

    gColor = vec4(baseColor, 1);
    gNormal = vec3(norm);
    gMaterial = vec4(material, 0, 0);
}

