#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

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

class Particle
{
private:
	GLuint id;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 gravity = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::vec3 gravity = glm::vec3(0.0f, -0.01f, 0.0f); // gravity -9.8f
	glm::vec3 pressure = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 viscosty = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 collisionState = glm::vec3(0.0f, 0.0f, 0.0f);

	GLfloat mass = 0.02f;
	GLfloat density = 0.0f;
	GLfloat p = 0.0f;

	GLfloat lifetime;
	GLfloat age;
public:
	std::list<SmoothDistance> distanceList;

	Particle();
	void setPos(glm::vec3 _pos);
	void setVelocity(glm::vec3 _v);
	void setID(GLuint _id);
	void setDensity(GLfloat _density);
	void setP(GLfloat _p);
	void setPressure(glm::vec3 _pressure);
	void setViscosity(glm::vec3 _viscosity);

	void setCollisionState(glm::vec3 _state);
	void resetCollisionState();

	GLint getID() { return id; }
	glm::vec3 getPos() { return position; }
	glm::vec3 getVelocity() { return velocity; }
	glm::vec3 getGravity() { return gravity; }
	GLfloat getMass() { return mass; }
	glm::vec3 getCollisionState() { return collisionState; }
	GLfloat getDensity() { return density; }
	GLfloat getP() { return p; }
	glm::vec3 getPressure() { return pressure; }
	glm::vec3 getViscosity() { return viscosty; }

	virtual ~Particle();
};

inline Particle::Particle()
{

}

inline void Particle::setPos(glm::vec3 _pos)
{
	position = _pos;
}

inline void Particle::setVelocity(glm::vec3 _v)
{
	velocity = _v;
}

inline void Particle::setID(GLuint _id)
{
	id = _id;
}

inline void Particle::setDensity(GLfloat _density)
{
	density = _density;
}


inline void Particle::setP(GLfloat _p)
{
	p = _p;
}

inline void Particle::setPressure(glm::vec3 _pressure)
{
	pressure = _pressure;
}

inline void Particle::setViscosity(glm::vec3 _viscosity)
{
	viscosty = _viscosity;
}

inline void Particle::setCollisionState(glm::vec3 _state)
{
	collisionState = _state;
}


inline void Particle::resetCollisionState()
{
	collisionState = glm::vec3(0.0f, 0.0f, 0.0f);
}

inline Particle::~Particle()
{

}

#endif
