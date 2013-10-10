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
#include "loadBMP.h"


//Position
glm::vec3 position   = glm::vec3( 0, 0, 5);
// horizontal angle : toward -Z;
float horizontalAngle = 3.14f;
// vertical angle : 0, look at the horizon
float verticalAngle  = 0.0f;
// Initial field of view
float initialFoV     = 45.0f;

float speed          = 3.0f; // 3 units/second;
float mouseSpeed     = 0.5f;

double currentTime = glfwGetTime();
double lastTime = glfwGetTime();
glm::mat4 projection_matrix;
glm::mat4 view_matrix;
glm::vec3 direction;

void computeMatricesFromInputs( )
{
	lastTime = currentTime;
	currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	float FoV;

	int xpos, ypos;
	int width, height;
	glfwGetMousePos( &xpos, &ypos );

	glfwGetWindowSize( &width, &height );
	glfwSetMousePos( width/2, height/2 );

	horizontalAngle += mouseSpeed * deltaTime * float(width/2 - xpos);
	verticalAngle   += mouseSpeed * deltaTime * float(height/2 - ypos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	direction = glm::vec3(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
		);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f),
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);

	// Up vector : perpendicular to both direction and right
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	FoV = initialFoV - 5 * glfwGetMouseWheel();

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projection_matrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	view_matrix = glm::lookAt(
		position,           // Camera is here
		position+direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
}

glm::mat4 getProjectionMatrix()
{
	return projection_matrix;
}

glm::mat4 getViewMatrix()
{
	return view_matrix;
}

glm::vec3 getCameraDirection()
{
	return direction; 
}