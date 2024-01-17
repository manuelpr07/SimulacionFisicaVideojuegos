#pragma once
#include "ParticleGenerator.h"
class UniformParticleGenerator :public ParticleGenerator
{
public:
	UniformParticleGenerator(Vector3 velWidth, Particle* model);
	~UniformParticleGenerator();

	Particle* generateParticles(std::list<Particle*>& particles, float t);
	Particle* generate(std::list<Particle*>& particles, Vector3 pos, Vector3 v, int mass);
	std::list <Particle*> listParticles;
	std::list <Particle*> activeParticles();
	void deleteParticle(Particle* p);
	void integrate(float t);

protected: 
	Vector3 velWidth;
};

