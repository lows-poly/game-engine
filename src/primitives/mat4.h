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

typedef float mat4[4][4];

static inline void mat4_set_zero( mat4 mat )
{
	int i, j;

	for ( i = 0; i < 4; i++ )
		for ( j = 0; j < 4; j++ )
			mat[i][j] = 0.0f;
}

static inline void mat4_identity( mat4 mat )
{
	mat[0][0] = 1.0f;	mat[1][0] = 0.0f;
	mat[0][1] = 0.0f;	mat[1][1] = 1.0f;
	mat[0][2] = 0.0f;	mat[1][2] = 0.0f;
	mat[0][3] = 0.0f;	mat[1][3] = 0.0f;

	mat[2][0] = 0.0f;	mat[3][0] = 0.0f;
	mat[2][1] = 0.0f;	mat[3][1] = 0.0f;
	mat[2][2] = 1.0f;	mat[3][2] = 0.0f;
	mat[2][3] = 0.0f;	mat[3][3] = 1.0f;
}

static inline void mat4_mul( const mat4 a, const mat4 b, mat4 dest )
{
	int col, row, k;
	float sum;

	for ( col = 0; col < 4; col++ ) {
		for ( row = 0; row < 4; row++ ) {
			sum = 0.0f;
			for ( k = 0; k < 4; k++ )
				sum += a[k][row] * b[col][k];
			dest[col][row] = sum;
		}
	}
}

static inline void mat4_ortho( float left, float right, float bottom, float top,
                               float znear, float zfar, mat4 dest )
{
	mat4_identity( dest );

	dest[0][0] = 2.0f / ( right - left );
	dest[1][1] = 2.0f / ( top - bottom );
	dest[2][2] = -2.0f / ( zfar - znear );

	dest[3][0] = -( right + left ) / ( right - left );
	dest[3][1] = -( top + bottom ) / ( top - bottom );
	dest[3][2] = -( zfar + znear ) / ( zfar - znear );
}

static inline void mat4_scale( mat4 mat, float scale )
{
	mat[0][0] *= scale;	mat[1][0] *= scale;
	mat[0][1] *= scale;	mat[1][1] *= scale;
	mat[0][2] *= scale;	mat[1][2] *= scale;
	mat[0][3] *= scale;	mat[1][3] *= scale;

	mat[2][0] *= scale;	mat[3][0] *= scale;
	mat[2][1] *= scale;	mat[3][1] *= scale;
	mat[2][2] *= scale;	mat[3][2] *= scale;
	mat[2][3] *= scale;	mat[3][3] *= scale;
}

static inline void mat4_translate( mat4 mat, float x, float y, float z )
{
	mat[3][0] += mat[0][0] * x + mat[1][0] * y + mat[2][0] * z;
	mat[3][1] += mat[0][1] * x + mat[1][1] * y + mat[2][1] * z;
	mat[3][2] += mat[0][2] * x + mat[1][2] * y + mat[2][2] * z;
	mat[3][3] += mat[0][3] * x + mat[1][3] * y + mat[2][3] * z;
}

static inline void mat4_rotate_z( mat4 mat, float angle_rad )
{
	float c, s, a0, a1;
	int row;

	c = cosf( angle_rad );
	s = sinf( angle_rad );

	for ( row = 0; row < 4; row++ ) {
		a0 = mat[0][row];
		a1 = mat[1][row];

		mat[0][row] = a0 * c + a1 * s;
		mat[1][row] = -a0 * s + a1 * c;
	}
}

static inline void mat4_scale_3f( mat4 mat, float x, float y, float z )
{
	mat[0][0] *= x; mat[0][1] *= x; mat[0][2] *= x; mat[0][3] *= x;
	mat[1][0] *= y; mat[1][1] *= y; mat[1][2] *= y; mat[1][3] *= y;
	mat[2][0] *= z; mat[2][1] *= z; mat[2][2] *= z; mat[2][3] *= z;
}

#endif
