#pragma once
#ifndef CONTAINER_H
#define CONTAINER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <vector>

// Define the container as a cube
class Container
{
public:
	GLfloat length;
	GLfloat width;
	GLfloat height;

	GLfloat vertices[288];

	Container();
	Container(GLfloat _length, GLfloat _width, GLfloat _height);
	void addVertices(GLfloat _length, GLfloat _width, GLfloat _height);
	virtual ~Container();
};

inline Container::Container()
{
	length = 1.0f;
	width = 1.0f;
	height = 1.0f;

	addVertices(length, height, width);
}

inline Container::Container(GLfloat _length, GLfloat _width, GLfloat _height)
{
	length = _length;
	width = _width;
	height = _height;

	addVertices(length, height, width);
}

inline void Container::addVertices(GLfloat _length, GLfloat _height, GLfloat _width)
{
	GLfloat x = _length;
	GLfloat y = _height;
	GLfloat z = _width;
	std::vector<GLfloat> temp = {
		//   Vertices            Normal vertex       Texcoords
		// Back face
		-x / 2, -y / 2, -z / 2, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
		 x / 2,  y / 2, -z / 2, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f, // Top-right
		 x / 2, -y / 2, -z / 2, 0.0f,  0.0f, 1.0f, 1.0f, 0.0f, // Bottom-right
		 x / 2,  y / 2, -z / 2, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f, // Top-right
		-x / 2, -y / 2, -z / 2, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f, // Bottom-left
		-x / 2,  y / 2, -z / 2, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, // Top-left
		// Front face
		-x / 2, -y / 2,  z / 2, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
		 x / 2, -y / 2,  z / 2, 0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // Bottom-right
		 x / 2,  y / 2,  z / 2, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // Top-right
		 x / 2,  y / 2,  z / 2, 0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // Top-right
		-x / 2,  y / 2,  z / 2, 0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // Top-left
		-x / 2, -y / 2,  z / 2, 0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // Top-right
		// Left face
		-x / 2,  y / 2,  z / 2, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // Top-right
		-x / 2,  y / 2, -z / 2, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // Top-left
		-x / 2, -y / 2, -z / 2, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // Bottom-left
		-x / 2, -y / 2, -z / 2, 1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // Bottom-left
		-x / 2, -y / 2,  z / 2, 1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // Bottom-rigth
		-x / 2,  y / 2,  z / 2, 1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // Top-right
		// Right face
		 x / 2,  y / 2,  z / 2, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // Top-left
		 x / 2, -y / 2, -z / 2, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // Bottom-right
		 x / 2,  y / 2, -z / 2, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // Top-right
		 x / 2, -y / 2, -z / 2, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // Bottom-right
		 x / 2,  y / 2,  z / 2, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // Top-left
		 x / 2, -y / 2,  z / 2, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // Bottom-left
		// Bottom face
		-x / 2, -y / 2, -z / 2, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f, // Top-right
		 x / 2, -y / 2, -z / 2, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f, // Top-left
		 x / 2, -y / 2,  z / 2, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Bottom-left
		 x / 2, -y / 2,  z / 2, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Bottom-left
		-x / 2, -y / 2,  z / 2, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f, // Bottom-right
		-x / 2, -y / 2, -z / 2, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f, // Top-right
		// Front face
		-x / 2,  y / 2, -z / 2, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // Top-left
		 x / 2,  y / 2,  z / 2, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // Bottom-right
		 x / 2,  y / 2, -z / 2, 0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // Top-right
		 x / 2,  y / 2,  z / 2, 0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // Bottom-right
		-x / 2,  y / 2, -z / 2, 0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // Top-left
		-x / 2,  y / 2,  z / 2, 0.0f, -1.0f,  0.0f, 0.0f, 0.0f // Bottom-left
	};

	for (unsigned int i = 0; i < temp.size(); ++i)
	{
		vertices[i] = temp[i];
	}
}

inline Container::~Container()
{
	
}

#endif
