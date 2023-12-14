#pragma once
#include "ParticleGenerator.h"
class UniformParticleGenerator :public ParticleGenerator
{
public:
	UniformParticleGenerator(Vector3 posWidth, Vector3 velWidth, Particle* model, std::string name);
	~UniformParticleGenerator();

	Particle* generateParticles(std::list<Particle*>& particles, double t);
	std::list <Particle*> listParticles;
	std::list <Particle*> activeParticles();
	void deleteParticle(Particle* p);
	void integrate(float t);

protected: 
	Vector3 velWidth, posWidth;
};

