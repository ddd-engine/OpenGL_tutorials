// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

// Include GLEW
#include <GL/glew.h>
// Include GLFW
#include <GL/glfw.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "loadShader.h"

using namespace glm;

// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

// One color for each vertex. They were generated randomly.
static const GLfloat g_color_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

int main( void )
{
	GLuint vertex_buffer;
	GLuint color_buffer;

	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( 1024, 768, 0,0,0,0, 32,0, GLFW_WINDOW ) )
	{
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetWindowTitle( "Tutorial 01" );

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );

	GLuint vertex_array_id;
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data),g_color_buffer_data, GL_STATIC_DRAW);

	// Dark blue background
	glClearColor(0.4f, 0.0f, 0.4f, 0.0f);

	// Create and compile our GLSL program from the shaders
	GLuint program_id = LoadShaders( "vertex_shader.vsc", "fragment_shader.fsc" );
	
	//Projection matrix of 45 degres 4:3 aspect ration 0.1 near plane 100 far plane
	glm::mat4 projection = glm::perspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
	//Camera matrix
	glm::mat4 view = glm::lookAt(
								  glm::vec3( 4, 3, -3 ),
								  glm::vec3( 0, 0, 0 ),
								  glm::vec3( 0, 1, 0 )
					 );
	//Model matrix
	glm::mat4 model = glm::mat4(1.0f);
	
	// Model-view-projection matrix
	glm::mat4 mvp = projection * view * model;
	GLuint mvp_id = glGetUniformLocation( program_id, "mvp" );
	
	GLint vertex_buffer_location = glGetAttribLocation( program_id, "vertex_position_modelspace");
	GLint color_buffer_location = glGetAttribLocation( program_id, "vertex_color");
	std::cout<<"Vertex buffer location: "<< vertex_buffer_location << std::endl;
	std::cout<<"Color buffer location: "<< vertex_buffer_location << std::endl;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	do{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		glUseProgram( program_id );		

		glUniformMatrix4fv( mvp_id, 1, GL_FALSE, &mvp[0][0] );
		glEnableVertexAttribArray(vertex_buffer_location);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
		glVertexAttribPointer( vertex_buffer_location, // Vertex Attrib Array enabled
							   3,					   // Number of elements that form an input in the shader (ex. 3 if shader has vec3 input)
							   GL_FLOAT,			   // Size of elements
							   GL_FALSE,			   // normalized
							   0,					   // stride
							   0					   // Offset in the array
							 );

		glEnableVertexAttribArray(color_buffer_location);
		glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
		glVertexAttribPointer( color_buffer_location, // Vertex Attrib Array enabled
							   3,					   // Number of elements that form an input in the shader (ex. 3 if shader has vec3 input)
							   GL_FLOAT,			   // Size of elements
							   GL_FALSE,			   // normalized
							   0,					   // stride
							   0					   // Offset in the array
							 );

		glDrawArrays(GL_TRIANGLES, 0, 12*3);
		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey( GLFW_KEY_ESC ) != GLFW_PRESS &&
		   glfwGetWindowParam( GLFW_OPENED ) );

	glDeleteProgram( program_id );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

