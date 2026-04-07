#version 410 core

out vec4 FragColor;

in float Height;
in vec4 WorldPos; // Must be passed from Vertex Shader



void main()
{





    float h = (Height + 16.0) / 32.0; // Your existing scale
    h = clamp(h, 0.0, 1.0);           // Safety check to keep h between 0 and 1

    vec3 darkGreen = vec3(0.05, 0.2, 0.05);  // Deep valley color
    vec3 lightGreen = vec3(0.3, 0.8, 0.3);   // High peak color

    vec3 finalColor = mix(darkGreen, lightGreen, h);
    if (h > 0.8)
        finalColor = vec3(1.0, 1.0, 1.0);

    // if (h < 0.2)
    //     finalColor = vec3(0.0,0.0,0.8);
    
    FragColor = vec4(finalColor, 1.0);
}