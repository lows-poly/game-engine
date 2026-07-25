/*
 * window.h - GLFW window creation and lifecycle management
 *
 * Handles GLFW/glad initialisation, window creation, etc.
 *
 * NOTE:
 *	Using int as boolean: 0 = false, 1 = true
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int window_init( int width, int height, const char *title );
int window_should_close( void );
void window_set_vsync( int enabled );
void window_set_target_fps( int fps );
void window_close( void );

#endif
