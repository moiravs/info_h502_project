#version 330 core
precision mediump float;

in vec2 v_tex;
out vec4 FragColor;

uniform sampler2D renderedTexture;
uniform float iTime;

void main(){
	//vec4 col = texture(renderedTexture, v_tex+vec2(0.8,0.0));
	vec4 col = texture(renderedTexture, v_tex);
	//col.w = 0.5;
	FragColor = col;
}
