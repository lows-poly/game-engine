#ifndef BASIC_2D_H
#define BASIC_2D_H

static const char *basic_2d_vsh_src =
	"#version 410 core\n"

	"layout ( location = 0 ) in vec2 a_pos;\n"
	"layout ( location = 1 ) in vec2 a_texcoord;\n"

	"uniform mat4 u_projection;\n"
	"uniform mat4 u_model;\n"

	"void main ()\n"
	"{\n"
	"	gl_Position = u_projection * u_model * vec4( a_pos, 0.0, 1.0 );\n"
	"	v_texcoord = a_texcoord;\n"
	"}\n";

static const char *basic_2d_fsh_src =
	"#version 410 core\n"

	"in vec2 v_texcoord;\n"

	"uniform sampler2D u_texture;\n"
	"uniform vec4 u_colour;\n"

	"out vec4 frag_colour;\n"

	"void main ()\n"
	"{\n"
	"	frag_colour = texture( u_texture, v_texcoord ) * u_colour;\n"
	"}\n";

#endif
