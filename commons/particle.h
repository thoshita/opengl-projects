//
// Created by Turfa Auliarachman on 15/05/18.
//

#ifndef OPENGL_2_PARTICLE_H
#define OPENGL_2_PARTICLE_H

#include <external/glm-0.9.7.1/glm/vec3.hpp>

struct Particle{
	glm::vec3 pos, speed;
	unsigned char r,g,b,a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particle& that) const {
		return this->cameradistance > that.cameradistance;
	}
};

#endif
