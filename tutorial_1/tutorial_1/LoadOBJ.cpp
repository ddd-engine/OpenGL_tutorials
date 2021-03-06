// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GL/glfw.h>

// Include GLM
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

bool loadOBJ( const char* path,
			 std::vector< glm::vec3 >   & out_vertices,
			 std::vector< glm::vec2 >   & out_uvs,
			 std::vector< glm::vec3 >   & out_normals,
			 std::vector< unsigned int> & out_indices
			 )
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	FILE *file = fopen( path, "r" );
	if( file == NULL )
	{
		std::cout<<"Unable to open file "<<file<<std::endl;
		return false;
	}

	while(1)
	{
		char lineHeader[128];
		//read first word of the line
		int res = fscanf( file, "%s", lineHeader );
		if( res == EOF )
			break;  //End of file reached

	
		if( strcmp( lineHeader, "v" ) == 0 )
		{
			//Read a new vertex
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if( strcmp( lineHeader, "vt" ) == 0 )
		{
			//Read a new uv coordinate
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if( strcmp( lineHeader, "vn" ) == 0 )
		{
			//Read a new vertex
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if( strcmp( lineHeader, "f" ) == 0 )
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			
			int matches = fscanf( file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
								  &vertexIndex[0], &uvIndex[0], &normalIndex[0],
								  &vertexIndex[1], &uvIndex[1], &normalIndex[1], 
								  &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if( matches != 9 )
			{
				std::cout<<"This OBJ format is not supported.\n"<<std::endl;
				return false;
			}
			
			for( unsigned int ind_in_face = 0; ind_in_face < 3; ind_in_face++ )
			{
				bool found = false;
				for(unsigned int i = 0 ; i < out_vertices.size(); i++ )
				{
					if( out_vertices[i] == temp_vertices[vertexIndex[ind_in_face]-1] &&
						out_uvs[i]      == temp_uvs[uvIndex[ind_in_face]-1]          &&
						out_normals[i]  == temp_normals[normalIndex[ind_in_face]-1] )
					{
						out_indices.push_back(i);
						found = true;
					}
				}
				if( !found )
				{
					out_vertices.push_back( temp_vertices[vertexIndex[ind_in_face]-1] );
					out_uvs.push_back( temp_uvs[uvIndex[ind_in_face]-1] );
					out_normals.push_back( temp_normals[normalIndex[ind_in_face]-1]);
					out_indices.push_back(out_vertices.size()-1);
				}
			}
		}
	}
	return true;
}
