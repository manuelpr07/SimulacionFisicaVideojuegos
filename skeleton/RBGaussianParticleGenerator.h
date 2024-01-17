#pragma once
#include <string>
#include <iostream>
#include "RenderUtils.hpp"
#include "RBParticle.h"
#include <list>
#include <random>
#include "ForceRegistry.h"

class RBGaussianParticleGenerator
{
public:
	RBGaussianParticleGenerator(RBParticle* mod, Vector3 posW, Vector3 velW, int max, PxPhysics* phys, PxScene* scene);
	~RBGaussianParticleGenerator();

	RBParticle* generateParticles(std::list<RBParticle*>& particles);
	std::list <RBParticle*> listParticles;

protected:
	Vector3 pos;
	Vector3 vel;
	Vector3 desvPos;
	Vector3 linearVel;
	Vector3 desvVel;
	Vector3 angularVel;
	int mass;
	PxShape* shape;
	Vector4 color;

	int maxParticles, currentParticles;
	PxPhysics* physics;
	PxScene* gScene;

	double _generation_prob = 1.0; // IF 1.0 --> always produces particles
	RBParticle* modelParticle = nullptr; // Has the attributes of the particle that will be generated!(damping, lifetime, etc.)
	Vector3 _origin, _mean_velocity;
	std::mt19937 mt;
	std::uniform_real_distribution<double> _u{ -1,1 };
	std::string _name;
};

