#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Particle* mod)
{
	_model_particle =  new Particle(mod->getPos(), mod->getVel(), mod->getMass(), mod->getDamping(), mod->getLifeTime());
	std::string name;
	_origin = _model_particle->getPos();
	_mean_velocity = _model_particle->getVel();
}

