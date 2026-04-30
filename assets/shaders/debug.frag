#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D depthMap;

void main() {             
    float depthValue = texture(depthMap, TexCoords).r;
    // We visualize it as grayscale. 
    // Note: If using Ortho, it's already linear. If Perspective, you'd need to linearize.
    
    FragColor = vec4(vec3(depthValue), 1.0);

}