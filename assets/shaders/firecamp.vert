 #version 410 core
in vec3 position; 
in vec2 tex_coord; 
in vec3 normal; 

out vec2 v_t;
out vec3 v_fragPos; 
out vec3 v_normal; 

uniform mat4 itM; // Use this instead of calculating it every vertex
uniform mat4 model; 

layout(std140) uniform CameraInfo {
     mat4 projection;
     mat4 view;
     vec4 camPosition;
     vec4 camRight;
     vec4 camUp;
 };

    void main(){ 
vec4 worldPos = model*vec4(position, 1.0); 
// 4. transfomr correctly the normals
v_normal = vec3(itM * vec4(normal, 1.0)); 
v_fragPos = worldPos.xyz; 
gl_Position = projection*view*worldPos; 
v_t = tex_coord;

    // same component in every direction
}