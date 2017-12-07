#pragma once
#ifndef KERNEL_H
#define KERNEL_H

void CalculateDensity(GLuint index, Particle* particles, GLfloat smoothLength, GLuint numParticles)
{
	GLuint id = particles[index].getID();
	GLfloat mass = particles[index].getMass();
	glm::vec3 position = particles[index].getPos();
	glm::vec3 dis = glm::vec3();
	GLfloat length = 0.0f;
	GLfloat densityKernel = 0.0f;
	GLfloat density = 0.0f;
	SmoothDistance temp;

	if (!particles[id].distanceList.empty())
	{
		particles[id].distanceList.clear();
	}

	for (GLuint i = 0; i != numParticles; i++)
	{
		if (index != i)
		{
			dis = position - particles[i].getPos();
			length = glm::sqrt(glm::dot(dis, dis));

			if (length <= smoothLength && length >= 0.0f)
			{
				temp.setID(i);
				temp.setDis(dis);
				temp.setLength(length);
				//distanceList.push_back(temp);
				particles[id].distanceList.push_back(temp);
			}
		}
	}

	for (std::list<SmoothDistance>::iterator iter = particles[id].distanceList.begin(); iter != particles[id].distanceList.end(); ++iter)
	{
		densityKernel = 315 / (64 * glm::pi<float>() * glm::pow(smoothLength, 9)) * glm::pow((smoothLength * smoothLength - glm::dot(iter->getDis(),iter->getDis())), 3);
		density += mass * densityKernel;
	}

	particles[index].setDensity(density);
}

void CalculatePressure(GLuint index, Particle* particles)
{
	GLfloat stableDensity = 1000.0f;
	GLfloat k = 1e-7;
	GLfloat density = particles[index].getDensity();
	GLfloat pressure = k * (density - stableDensity);
	particles[index].setP(pressure);
}

void PressureKernel(GLuint index, Particle* particles, GLfloat smoothLength)
{
	GLuint id = index;
	glm::vec3 pressure = glm::vec3();

	for (std::list<SmoothDistance>::iterator iter = particles[id].distanceList.begin(); iter != particles[id].distanceList.end(); ++iter)
	{
		GLfloat i = iter->getLength();
		glm::vec3 grad = (-45 / (glm::pi<float>() * glm::pow(smoothLength, 6)) * glm::pow((smoothLength - iter->getLength()), 2)) * (glm::normalize(iter->getDis()));
		pressure += particles[id].getMass() * (particles[id].getP() + particles[iter->getID()].getP()) / (2 * particles[iter->getID()].getDensity()) * grad;
	}

	particles[id].setPressure(-pressure);
}

void ViscosityKernel(GLfloat index, Particle* particles, GLfloat smoothLength)
{
	GLuint id = index;
	GLfloat mass = particles[id].getMass();
	glm::vec3 velocity = particles[id].getVelocity();
	glm::vec3 viscosity = glm::vec3();
	GLfloat mu = 9.01e-6f;

	for (std::list<SmoothDistance>::iterator iter = particles[id].distanceList.begin(); iter != particles[id].distanceList.end(); ++iter)
	{
		glm::vec3 relativeVelocity = particles[iter->getID()].getVelocity() - velocity;
		GLfloat laplacian = 45 / (glm::pi<float>() * glm::pow(smoothLength, 6)) * (smoothLength - iter->getLength());

		viscosity += mass * relativeVelocity / particles[iter->getID()].getDensity() * laplacian;
	}

	viscosity *= mu;
	particles[id].setViscosity(viscosity);
}

#endif
