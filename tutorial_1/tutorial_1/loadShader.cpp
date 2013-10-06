#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>

GLuint LoadShaders( char* vertex_shader_path, char* fragment_shader_path)
{
	//Create Shaders
	GLuint vertex_shader_id   = glCreateShader( GL_VERTEX_SHADER );
	GLuint fragment_shader_id = glCreateShader( GL_FRAGMENT_SHADER );

	// Read vertex shader code from file
	std::string vertex_shader_code;
	std::ifstream vertex_shader_stream( vertex_shader_path );
	if( vertex_shader_stream.is_open())
	{
		std::string line ="";
		while( getline(vertex_shader_stream, line))
			vertex_shader_code += "\n" + line;
		vertex_shader_stream.close();
	}
	else
	{
		std::cout<<"Error while opening vertex shader code"<<std::endl;
	}

	std::string fragment_shader_code;
	std::ifstream fragment_shader_stream( fragment_shader_path, std::ios::in );
	if( fragment_shader_stream.is_open())
	{
		std::string line ="";
		while( getline(fragment_shader_stream, line))
			fragment_shader_code += "\n" + line;
		fragment_shader_stream.close();
	}
	else
	{
		std::cout<<"Error while opening fragment shader code"<<std::endl;
	}

	GLint result = GL_FALSE;
	int info_log_length;

	//compile vertex shader
	std::cout<<" Compiling Vertex shader from "<< vertex_shader_path;
	char const * vertex_shader_code_ptr = vertex_shader_code.c_str();
	glShaderSource( vertex_shader_id, 1, &vertex_shader_code_ptr, NULL);
	glCompileShader( vertex_shader_id );

	//check compilation
	glGetShaderiv( vertex_shader_id, GL_COMPILE_STATUS, &result );
	glGetShaderiv( vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length );
	std::vector<char> vertex_shader_log_message(info_log_length);
	glGetShaderInfoLog(vertex_shader_id, info_log_length, NULL, &vertex_shader_log_message[0] );
	std::cout<<&vertex_shader_log_message[0]<<std::endl;

	//compile vertex shader
	std::cout<<" Compiling Fragment shader from "<< fragment_shader_path;
	char const * fragment_shader_code_ptr = fragment_shader_code.c_str();
	glShaderSource( fragment_shader_id, 1, &fragment_shader_code_ptr, NULL);
	glCompileShader( fragment_shader_id );

	//check compilation
	glGetShaderiv( fragment_shader_id, GL_COMPILE_STATUS, &result );
	glGetShaderiv( fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length );
	std::vector<char> fragment_shader_log_message(info_log_length);
	glGetShaderInfoLog(fragment_shader_id, info_log_length, NULL, &fragment_shader_log_message[0] );
	std::cout<<&fragment_shader_log_message[0]<<std::endl;

	//Link program
	std::cout<< "Linking program "<<std::endl;

	GLuint program_id = glCreateProgram();
	glAttachShader( program_id, vertex_shader_id );
	glAttachShader( program_id, fragment_shader_id );
	glLinkProgram( program_id );

	//Check Program;
	glGetProgramiv( program_id, GL_LINK_STATUS, &result );
	glGetProgramiv( program_id, GL_INFO_LOG_LENGTH, &info_log_length );
	std::vector<char> program_log_message(info_log_length);
	glGetProgramInfoLog( program_id, info_log_length, NULL, &program_log_message[0]);
	std::cout<<&program_log_message[0]<<std::endl;

	glDeleteShader( vertex_shader_id );
	glDeleteShader( fragment_shader_id );
	
	return program_id;
}