#ifndef SHADER_H
#define SHADER_H

#include <stddef.h>
#include <glad/glad.h>

#include <primitives/vec2.h>
#include <primitives/ivec3.h>
#include <primitives/vec3.h>
#include <primitives/vec4.h>
#include <primitives/mat4.h>

#define SHADER_UNIFORM_CACHE_MAX        32
#define SHADER_UNIFORM_NAME_MAX         64

struct shader_uniform {
	char name[SHADER_UNIFORM_NAME_MAX];
	GLint loc;
};

struct shader {
	GLuint id;

	struct shader_uniform uniforms[SHADER_UNIFORM_CACHE_MAX];
	size_t u_count;
};

enum shader_builtin {
	SHADER_DEFAULT,
	SHADER_PRIMITIVE_2D,
};

int shader_init( struct shader *s, const char *vert_path, const char *frag_path );
int shader_init_preset( struct shader *s, enum shader_builtin preset );
void shader_use( const struct shader *s );

/* UNIFORM SETTING FUNCTIONS */
int shader_set_1i( struct shader *s, const char *name, int value );
int shader_set_1f( struct shader *s, const char *name, float value );

int shader_set_2f( struct shader *s, const char *name, const float value[2] );
int shader_set_vec2( struct shader *s, const char *name, vec2 v );

int shader_set_3i( struct shader *s, const char *name, const int value[3] );
int shader_set_ivec3( struct shader *s, const char *name, ivec3 v );

int shader_set_3f( struct shader *s, const char *name, const float value[3] );
int shader_set_vec3( struct shader *s, const char *name, vec3 v );

int shader_set_4f( struct shader *s, const char *name, const float value[4] );
int shader_set_vec4( struct shader *s, const char *name, vec4 v );

int shader_set_mat4( struct shader *s, const char *name, const mat4 mat );

void shader_destroy( struct shader *s );

#endif
