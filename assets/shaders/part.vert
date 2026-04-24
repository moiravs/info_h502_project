#version 330 core
in vec3 vertex;
in vec4 center;
in vec4 col;

layout(std140) uniform CameraInfo {
	mat4 projection;
	mat4 view;
	vec4 camPosition;
	vec4 camRight;
	vec4 camUp;
};

out vec4 color;

void main(){
	//center is x,y,z pos of center + w for the size of the particle
	float scale = center.w;

	vec3 particleCenter = center.xyz;

	//replace the usage of model matrice with just a translation for the position 
	vec3 vertexPosition_worldspace = particleCenter + camRight.xyz * vertex.x * scale + camUp.xyz * vertex.y * scale;
	

	gl_Position = projection*view*vec4(vertexPosition_worldspace, 1.0);
	color = col;
}