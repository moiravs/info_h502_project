#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
// mat4 takes up 4 slots: 3, 4, 5, and 6
layout (location = 3) in mat4 instanceMatrix; 

uniform mat4 lightSpaceMatrix;

out vec2 v_t;

void main()
{
    v_t = aTexCoords;
    // Use instanceMatrix (from the VBO) instead of the uniform 'model'
    gl_Position = lightSpaceMatrix * instanceMatrix * vec4(aPos, 1.0);
}