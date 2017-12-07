#pragma once
#ifndef SMOOTHDISTANCE_H
#define SMOOTHDISTANCE_H

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

class SmoothDistance
{
private:
	GLuint id = 0;
	glm::vec3 dis;
	GLfloat length;
public:
	SmoothDistance();

	void setID(GLuint _id);
	void setDis(glm::vec3 _dis);
	void setLength(GLfloat _length);

	GLuint getID() { return id; }
	glm::vec3 getDis() { return dis; }
	GLfloat getLength() { return length; }

	virtual ~SmoothDistance();
};

inline SmoothDistance::SmoothDistance()
{
	dis = glm::vec3(0.0f, 0.0f, 0.0f);
	length = 0.0f;
}

inline void SmoothDistance::setID(GLuint _id)
{
	id = _id;
}

inline void SmoothDistance::setDis(glm::vec3 _dis)
{
	dis = _dis;
}

inline void SmoothDistance::setLength(GLfloat _length)
{
	length = _length;
}

inline SmoothDistance::~SmoothDistance()
{

}

#endif
