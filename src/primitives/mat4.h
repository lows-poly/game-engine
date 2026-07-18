#ifndef ENGINE_MAT4_H
#define ENGINE_MAT4_H

#include <math.h>
#include "vec3.h"

#define MAT4_ZERO_INIT		{ 0.0f, 0.0f, 0.0f, 0.0f,  \
                                  0.0f, 0.0f, 0.0f, 0.0f,  \
                                  0.0f, 0.0f, 0.0f, 0.0f,  \
                                  0.0f, 0.0f, 0.0f, 0.0f } 

#define MAT4_ZERO		((mat4)MAT4_ZERO_INIT)

#define MAT4_IDENTITY_INIT	{ 1.0f, 0.0f, 0.0f, 0.0f,  \
                                  0.0f, 1.0f, 0.0f, 0.0f,  \
                                  0.0f, 0.0f, 1.0f, 0.0f,  \
                                  0.0f, 0.0f, 0.0f, 1.0f } 

#define MAT4_IDENTITY		((mat4)MAT4_IDENTITY_INIT)

typedef struct mat4 {
	float m[16];
} mat4;

static inline void mat4_zero( mat4 *r )
{
	for ( int i = 0; i < 16; i++ ) {
		r->m[i] = 0.0f;
	}
}

static inline mat4 mat4_identity( void )
{
	mat4 r = MAT4_ZERO;

	r.m[0] = 1.0f;
	r.m[5] = 1.0f;
	r.m[10] = 1.0f;
	r.m[15] = 1.0f;

	return r;
}

static inline mat4 mat4_multiply( mat4 a, mat4 b )
{
	mat4 r;

	for ( int col = 0; col < 4; col++ ) {
		for ( int row = 0; row < 4; row++ ) {
			float sum = 0.0f;

			for ( int k = 0; k < 4; k++ )
				sum += a.m[(k * 4) + row] * b.m[(col * 4) + k];

			r.m[(col * 4) + row] = sum;
		}
	}

	return r;
}

static inline mat4 mat4_translate( vec3 t )
{
	mat4 r = MAT4_IDENTITY;

	r.m[12] = t.x;
	r.m[13] = t.y;
	r.m[14] = t.z;

	return r;
}

static inline mat4 mat4_scale( vec3 s )
{
	mat4 r = MAT4_IDENTITY;

	r.m[0] = s.x;
	r.m[5] = s.y;
	r.m[10] = s.z;

	return r;
}

static inline mat4 mat4_perspective( float fov_y, float aspect, float near,
                                     float far )
{
	mat4 r = MAT4_ZERO;
	float f = 1.0f / tanf(fov_y * 0.5f) ;
	
	r.m[0] = f / aspect;
	r.m[5] = f;
	r.m[10] = (far + near) / (near - far);
	r.m[11] = -1.0f;
	r.m[14] = (2.0f * far * near) / (near - far);

	return r;
}

static inline mat4 mat4_look_at( vec3 eye, vec3 centre, vec3 up )
{
	mat4 r;
	vec3 f = vec3_normalise( vec3_sub( centre, eye ) );
	vec3 s = vec3_normalise( vec3_cross( f, up ) );
	vec3 u = vec3_cross( s, f );
 
	r.m[0] = s.x;
	r.m[1] = u.x;
	r.m[2] = -f.x;
	r.m[3] = 0.0f;
 
	r.m[4] = s.y;
	r.m[5] = u.y;
	r.m[6] = -f.y;
	r.m[7] = 0.0f;
 
	r.m[8] = s.z;
	r.m[9] = u.z;
	r.m[10] = -f.z;
	r.m[11] = 0.0f;
 
	r.m[12] = -vec3_dot( s, eye );
	r.m[13] = -vec3_dot( u, eye );
	r.m[14] = vec3_dot( f, eye );
	r.m[15] = 1.0f;
 
	return r;
}

#endif
