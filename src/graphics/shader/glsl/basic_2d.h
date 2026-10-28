#ifndef BASIC_2D_H
#define BASIC_2D_H

static const char *basic_2d_vsh_src =
	"#version 410 core\n"

	"layout ( location = 0 ) in vec2 a_pos;\n"

	"uniform mat4 u_projection;\n"
	"uniform mat4 u_model;\n"

	"void main ()\n"
	"{\n"
	"	gl_Position = u_projection * u_model * vec4( a_pos, 0.0, 1.0 );\n"
	"}\n";

static const char *basic_2d_fsh_src =
	"#version 410 core\n"

	"uniform vec4 u_colour;\n"

	"out vec4 frag_colour;\n"

	"void main ()\n"
	"{\n"
	"	frag_colour = u_colour;\n"
	"}\n";

static const char *textured_2d_vsh_src =
	"#version 410 core\n"

	"layout ( location = 0 ) in vec2 a_pos;\n"
	"layout ( location = 1 ) in vec2 a_uv;\n"

	"uniform mat4 u_projection;\n"
	"uniform mat4 u_model;\n"
	"uniform vec2 u_uv_offset;\n"
	"uniform vec2 u_uv_scale;\n"

	"out vec2 vert_uv;\n"

	"void main ()\n"
	"{\n"
	"	gl_Position = u_projection * u_model * vec4( a_pos, 0.0, 1.0 );\n"
	"	vert_uv = u_uv_offset + a_uv * u_uv_scale;\n"
	"}\n";

static const char *textured_2d_fsh_src =
	"#version 410 core\n"

	"in vec2 vert_uv;\n"

	"uniform sampler2D u_texture;\n"
	"uniform vec4 u_tint;\n"

	"out vec4 frag_colour;\n"

	"void main ()\n"
	"{\n"
	"	frag_colour = texture( u_texture, vert_uv ) * u_tint;\n"
	"}\n";

#endif
