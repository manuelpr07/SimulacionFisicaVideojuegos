#include "RBGaussianParticleGenerator.h"

RBGaussianParticleGenerator::RBGaussianParticleGenerator(RBParticle* mod, Vector3 posW,Vector3 velW, int max, PxPhysics* phys, PxScene* scene)
{
	desvPos = posW;
	linearVel = linearVel;

	maxParticles = max;
	currentParticles = 0;
	physics = phys;
	gScene = scene;
	pos = mod->getPos().p;
	vel = mod->getVel();
	shape = mod->getShape();
	color = mod->getColor();
	mass = mod->getMass();
}

RBGaussianParticleGenerator::~RBGaussianParticleGenerator()
{
	auto it = listParticles.begin();
	while (it != listParticles.end()) {
		it = listParticles.erase(it);
	}
}

RBParticle* RBGaussianParticleGenerator::generateParticles(std::list<RBParticle*>& particles)
{

		Vector3 newPos;
		Vector3 newVel;

		std::mt19937 mt(std::random_device{}());

		//calculo de la nueva posicion
		std::normal_distribution<double> distributionPX(pos.x, desvPos.x);
		newPos.x = distributionPX(mt);

		std::normal_distribution<double> distributionPY(pos.y, desvPos.y);
		newPos.y = distributionPY(mt);

		std::normal_distribution<double> distributionPZ(pos.z, desvPos.z);
		newPos.z = distributionPZ(mt);

		//calculo de la nueva velocidad
		std::normal_distribution<double> distributionVX(vel.x, desvVel.x);
		newVel.x = distributionVX(mt);

		std::normal_distribution<double> distributionVY(vel.y, desvVel.y);
		newVel.y = distributionVY(mt);

		std::normal_distribution<double> distributionVZ(vel.z, desvVel.z);
		newVel.z = distributionVZ(mt);

		auto a = std::chrono::high_resolution_clock::now();
		double actualTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();

		RBParticle* part = new RBParticle(newPos, newVel, { 1, 1, 1 }, mass, 5, physics, gScene, shape, color);
		currentParticles++;
		particles.push_back(part);
		return part;

}
