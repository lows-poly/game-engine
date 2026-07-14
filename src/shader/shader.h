#ifndef SHADER_H
#define SHADER_H

struct shader {
	unsigned int id;
};

int shader_create( struct shader *s, const char *vert_path, const char *frag_path );
void shader_use( struct shader *shader_program );
void shader_destroy( struct shader *shader_program );

#endif SHADER_H
