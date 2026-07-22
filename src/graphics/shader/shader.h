#ifndef SHADER_H
#define SHADER_H

#include <stddef.h>
#include <glad/glad.h>

#include <primitives/vec3.h>

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

int shader_create( struct shader *s, const char *vert_path, const char *frag_path );
void shader_use( const struct shader *s );

int shader_set_int( struct shader *s, const char *name, int value );
int shader_set_float( struct shader *s, const char *name, float value );
int shader_set_3i( struct shader *s, const char *name, const int value[3] );
int shader_set_ivec3( struct shader *s, const char *name, ivec3 v );
int shader_set_3f( struct shader *s, const char *name, const float value[3] );
int shader_set_vec3( struct shader *s, const char *name, vec3 v );

int shader_destroy( struct shader *s );

#endif
