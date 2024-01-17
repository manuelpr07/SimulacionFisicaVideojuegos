#pragma once
#include <string>
#include <iostream>
#include "RenderUtils.hpp"
#include "RBParticle.h"
#include <list>
#include <random>
#include "ForceRegistry.h"

class RBUniformParticleGenerator
{
public:
	RBUniformParticleGenerator(RBParticle* mod, Vector3 velW, int max, PxPhysics* phys, PxScene* scene);
	~RBUniformParticleGenerator();

	RBParticle* generateParticles(std::list<RBParticle*>& particles, Vector3 pos, Vector3 v, int mass);
	std::list <RBParticle*> listParticles;

protected:
	Vector3 pos;
	Vector3 linearVel;
	Vector3 angularVel;
	int mass;

	int maxParticles, currentParticles;
	PxPhysics* physics;
	PxScene* gScene;

	RBParticle* _model_particle = nullptr; // Has the attributes of the particle that will be generated!(damping, lifetime, etc.)
	Vector3 velWidth;

	double _generation_prob = 1.0; // IF 1.0 --> always produces particles
	Vector3 _origin, _mean_velocity;
	std::mt19937 _mt;
	std::uniform_real_distribution<double> _u{ -1, 1 };
	std::string _name;
};

