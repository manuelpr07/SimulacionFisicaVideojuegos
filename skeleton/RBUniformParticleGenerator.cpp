#include "RBUniformParticleGenerator.h"
RBUniformParticleGenerator::RBUniformParticleGenerator(RBParticle* mod, Vector3 velW, int max, PxPhysics* phys, PxScene* scene)
{
	velWidth = velW;
	linearVel = linearVel;

	maxParticles = max;
	currentParticles = 0;
	physics = phys;
	gScene = scene;
	_model_particle = new RBParticle(mod->getPos().p, mod->getVel(), { 0,0,0 }, mod->getMass(), mod->getLifeTime(), physics, gScene, CreateShape(PxBoxGeometry(1, 1, 1)), mod->getColor());
	currentParticles++;
}

RBUniformParticleGenerator::~RBUniformParticleGenerator()
{
	auto it = listParticles.begin();
	while (it != listParticles.end()) {
		it = listParticles.erase(it);
	}
}

RBParticle* RBUniformParticleGenerator::generateParticles(std::list<RBParticle*>& particles, Vector3 pos, Vector3 v, int mass)
{
	//generar un rigidbody dinamico
	PxRigidDynamic* new_solid;

	PxShape* shape = CreateShape(PxBoxGeometry(1, 1, 1));
	auto a = std::chrono::high_resolution_clock::now();
	double actualTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();

	RBParticle* part = new RBParticle(pos, v, { 0, 0, 0 }, mass, 3, physics, gScene, shape, _model_particle->getColor());
	currentParticles++;
	particles.push_back(part);
	return part;

}
