#version 410 core

layout ( location = 0 ) in vec2 a_pos;

uniform mat4 u_projection;
uniform vec2 u_pos;
uniform vec2 u_scale;

void main ()
{
	vec2 world_pos = a_pos * u_scale + u_pos;
	gl_Position = u_projection * vec4( world_pos, 0.0, 1.0 );
}
