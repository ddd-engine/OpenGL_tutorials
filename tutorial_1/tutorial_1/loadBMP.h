#pragma once

GLuint LoadBMP( std::string bmp_filename );
GLuint LoadBMP_custom(const char* imagepath);
GLuint LoadTGA_glfw(const char* imagepath);
GLuint LoadDDS( const char* imagepath );