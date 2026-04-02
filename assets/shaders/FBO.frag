#version 420 core
//precision mediump float;


layout(location = 0) out vec4 color;

uniform sampler2D renderedTexture;

uniform vec2 iResolution;
uniform float iTime;
uniform vec2 iMouse;

float dPx = 1.0/iResolution.x;
float dPy = 1.0/iResolution.y;


float f = 0.25;

void main() {
	vec3 col;
	vec2 fragCoord = gl_FragCoord.xy;
	vec2 uv = fragCoord/iResolution.xy;
	
	vec3 prev = texture(renderedTexture, uv).rgb;

	vec3 right = texture(renderedTexture, uv+vec2(-dPx,0.0)).rgb;
	vec3 left = texture(renderedTexture, uv+vec2(dPx,0.0)).rgb;
	vec3 up = texture(renderedTexture, uv+vec2(0.0,dPy)).rgb;
	vec3 down = texture(renderedTexture, uv+vec2(0.0,-dPy)).rgb;
	
	vec3 factor = f*(right + left + up + down - 4*prev);


	float th = 0.003;
	if(factor.r >= -th && factor.r < 0.0) factor = vec3(-th);
	col = prev + factor;

	vec2 center = iMouse.xy;
	center.y = iResolution.y-center.y;
	
	float l = length(fragCoord - center );
	if(l<10) col = vec3(1.0, 0.5, 0.5);
	
	color = vec4(col, 1.0);
	
}









