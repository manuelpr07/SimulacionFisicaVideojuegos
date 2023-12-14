#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Particle* mod)
{
	_model_particle =  new Particle(mod->getPos().p, mod->getVel(), mod->getMass(), mod->getDamping(), mod->getLifeTime(), mod->getColor());
	std::string name;
	_origin = _model_particle->getPos().p;
	_mean_velocity = _model_particle->getVel();
}

