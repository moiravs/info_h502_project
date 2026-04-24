 #version 330 core
layout (location = 0) in vec3 position; 
layout (location = 1) in vec2 tex_coord; 
layout (location = 2) in vec3 normal; 

out vec3 v_frag_coord; 
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
vec4 frag_coord = model*vec4(position, 1.0); 
gl_Position = projection*view*frag_coord; 
// 4. transfomr correctly the normals
v_normal = vec3(itM * vec4(normal, 1.0)); 
v_frag_coord = frag_coord.xyz; 
    // same component in every direction
}