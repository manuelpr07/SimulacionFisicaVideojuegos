#pragma once

#include <string>
#include <iostream>
#include "RenderUtils.hpp"
#include "Particle.h"
#include <list>
#include <random>
#include "ForceRegistry.h"

using namespace physx;

class ParticleGenerator
{

public:
	ParticleGenerator(Particle* model);
	~ParticleGenerator() {};

	inline void setOrigin(const Vector3& p) { _origin = p; }
	inline void setMeanVelocity(const Vector3& v) {
		_mean_velocity
			= v;
	}
	inline Vector3 getMeanVelocity() const {
		return _mean_velocity;
	}
	inline void setMeanDuration(double new_duration) {
		_model_particle->setLifeTime(new_duration);
	}
	inline void setParticle(Particle* p, bool modify_pos_vel =
		true) {
		delete _model_particle;
		_model_particle = p;
		if (modify_pos_vel) {
			_origin = p->getPos().p;
			_mean_velocity = p->getVel();
		}
		_model_particle->setPos({ -1000.0f, -1000.0f, -1000.0f });
	}
	inline void setNParticles(int n_p) { _n_particles = n_p; }

	inline std::string getName() const { return _name; }

	virtual void integrate(float f) = 0;

	virtual std::list <Particle*> activeParticles() = 0;
	virtual Particle* generateParticles(std::list<Particle*>& particles, float t) = 0;
protected:
	int _n_particles = 3; // Number of particles for each generateParticles call(TODO: add randomness ? ? )
	double _generation_prob = 1.0; // IF 1.0 --> always produces particles
	Particle* _model_particle = nullptr; // Has the attributes of the particle that will be generated!(damping, lifetime, etc.)
	Vector3 _origin, _mean_velocity;
	std::mt19937 _mt;
	std::uniform_real_distribution<double> _u{ -1,1 };
	std::string _name;
};

