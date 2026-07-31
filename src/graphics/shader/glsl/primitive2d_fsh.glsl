#version 410 core

uniform vec4 u_colour;

out vec4 frag_colour;
void main ()
{
	frag_colour = u_colour;
}
