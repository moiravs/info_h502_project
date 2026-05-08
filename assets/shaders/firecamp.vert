 #version 410 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex;
layout(location = 2) in vec3 aNormal;

out vec2 v_t;
out vec3 v_fragPos; 
out vec3 v_normal;

uniform mat4 itM; 
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

out vec4 FragPosLightSpace;

layout(std140) uniform CameraInfo {
     mat4 projection;
     mat4 view;
     vec4 camPosition;
     vec4 camRight;
     vec4 camUp;
 };

    void main(){ 
vec4 worldPos = model * vec4(aPos, 1.0);
// 4. transfomr correctly the normals
v_normal = vec3(itM * vec4(aNormal, 1.0));
v_fragPos = worldPos.xyz; 
gl_Position = projection*view*worldPos; 
v_t = aTex;
FragPosLightSpace = lightSpaceMatrix * model * vec4(aPos, 1.0);
}