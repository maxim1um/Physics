#pragma once
#ifndef CHECKLIST_H
#define CHECKLIST_H

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
#include <list>

class Checklist
{
private:
	GLuint id;
	std::list<SmoothDistance> data;
public:
	Checklist();
	void setID(GLuint _id);
	void setData(std::list<SmoothDistance> _data);
	
	GLuint getID();
	std::list<SmoothDistance> getData();
	virtual ~Checklist();
};

inline Checklist::Checklist()
{
	id = 0;
}

inline void Checklist::setID(GLuint _id)
{
	id = _id;
}

inline void Checklist::setData(std::list<SmoothDistance> _data)
{
	data = _data;
}

inline GLuint Checklist::getID()
{
	return id;
}

inline std::list<SmoothDistance> Checklist::getData()
{
	return data;
}

inline Checklist::~Checklist()
{
	
}

#endif
