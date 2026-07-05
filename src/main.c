#include <stdlib.h>

#include <GLFW/glfw3.h>

int main( void ) {
	GLFWwindow* window;

	if ( !glfwInit() )
		return EXIT_FAILURE;

	window = glfwCreateWindow( 640, 480, "Hello World", NULL, NULL );

	if ( !window ) {
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent( window );

	while ( !glfwWindowShouldClose( window ) ) {
		glClear( GL_COLOR_BUFFER_BIT );

		glfwSwapBuffers( window );

		glfwPollEvents();
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}
