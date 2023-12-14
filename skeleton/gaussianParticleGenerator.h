#pragma once
#include "ParticleGenerator.h"
class gaussianParticleGenerator :public ParticleGenerator
{
public:
	gaussianParticleGenerator(Vector3 posW, Vector3 velW, Particle* model, std::string name);
	~gaussianParticleGenerator();

	Particle* generateParticles(std::list<Particle*>& particles, float t);
	std::list <Particle*> listParticles;
	std::list <Particle*> activeParticles();
	void deleteParticle(Particle* p);
	void integrate(float t);

protected:
	Vector3 desvVel;
	Vector3 desvPos;
	Vector3 area;

};

