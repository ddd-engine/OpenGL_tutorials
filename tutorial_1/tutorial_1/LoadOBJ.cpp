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

#include "utils.h"

bool loadOBJ( const char* path,
			 std::vector< glm::vec3 >   & out_vertices,
			 std::vector< glm::vec2 >   & out_uvs,
			 std::vector< glm::vec3 >   & out_normals,
			 std::vector< unsigned int> & out_indices,
			 std::vector< glm::vec3 >   & out_tangents,
			 std::vector< glm::vec3 >   & out_bitangents
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
			std::vector<unsigned int> vertexIndex(3);
			std::vector<unsigned int> uvIndex(3);
			std::vector<unsigned int> normalIndex(3);
			
			int matches = fscanf( file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
								  &vertexIndex[0], &uvIndex[0], &normalIndex[0],
								  &vertexIndex[1], &uvIndex[1], &normalIndex[1], 
								  &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if( matches != 9 )
			{
				std::cout<<"This OBJ format is not supported.\n"<<std::endl;
				return false;
			}

			glm::vec3 temp_bitangent, temp_tangent;
			computeTangentBasis(temp_vertices, vertexIndex, temp_uvs, uvIndex, temp_normals, normalIndex,temp_tangent, temp_bitangent);
			
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
						out_tangents[i]   += temp_tangent;
						out_bitangents[i] += temp_bitangent;
						found = true;
					}
				}
				if( !found )
				{
					out_vertices.push_back( temp_vertices[vertexIndex[ind_in_face]-1] );
					out_uvs.push_back( temp_uvs[uvIndex[ind_in_face]-1] );
					out_normals.push_back( temp_normals[normalIndex[ind_in_face]-1]);
					out_indices.push_back(out_vertices.size()-1);
					out_tangents.push_back(temp_tangent);
					out_bitangents.push_back(temp_bitangent);
				}
			}
		}
	}
	return true;
}
