#pragma once
#ifndef COLLISION_H
#define COLLISION_H

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

void UpdateState(GLuint index, Particle* particles, GLfloat deltatime)
{
	glm::vec3 force = particles[index].getGravity() + particles[index].getPressure() + particles->getViscosity();
	GLfloat mass = particles[index].getMass();
	glm::vec3 acceleration = force / mass;

	glm::vec3 velocity = particles[index].getVelocity();
	glm::vec3 position = particles[index].getPos();

	position += velocity * deltatime + (0.5f * acceleration * glm::pow(deltatime, 2));
	velocity += acceleration * deltatime;

	particles[index].setPos(position);
	particles[index].setVelocity(velocity);
}

#endif
