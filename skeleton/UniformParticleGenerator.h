#pragma once
#include "ParticleGenerator.h"
class UniformParticleGenerator :public ParticleGenerator
{
public:
	UniformParticleGenerator(Vector3 posWidth, Vector3 velWidth, Particle* model);
	~UniformParticleGenerator();

	void generateParticles(double t);
	std::list <Particle*> listParticles;
	std::list <Particle*> activeParticles();
	void deleteParticle(Particle* p);
	void integrate(float t);

protected: 
	Vector3 velWidth, posWidth;
};

