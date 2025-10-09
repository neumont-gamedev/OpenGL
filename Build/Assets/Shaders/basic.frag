#version 460 core

in vec3 v_color;
out vec4 f_color;

uniform float u_time;

void main()
{
	//f_color = vec4(1, 1, 0, 1);
	float offset = sin(u_time + gl_FragCoord.x);
	f_color = vec4(v_color / offset, 1);
}