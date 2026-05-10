 #version 330 core
layout(location = 0) in vec3 aPos;
 layout(location = 1) in vec2 aTex;
 layout(location = 2) in vec3 aNormal;

out vec3 v_normal;

uniform mat4 model; 
uniform mat4 itM;

 layout(std140) uniform CameraInfo {
     mat4 projection;
     mat4 view;
     vec4 camPosition;
     vec4 camRight;
     vec4 camUp;
 };

    void main(){ 
    vec4 frag_coord = model * vec4(aPos, 1.0);
    gl_Position = projection*view*frag_coord;
    v_normal = vec3(itM * vec4(aNormal, 1.0));
}