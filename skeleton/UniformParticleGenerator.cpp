#include "UniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(Vector3 velW, Particle* m): ParticleGenerator(m)
{
	velWidth = velW;
}
UniformParticleGenerator::~UniformParticleGenerator()
{

	for (Particle* part : activeParticles())
		delete part;

}

Particle* UniformParticleGenerator::generateParticles(std::list<Particle*>& particles, float t)
{
	return nullptr;
}

Particle* UniformParticleGenerator::generate(std::list<Particle*>& particles, Vector3 pos, Vector3 v, int mass)
{

	PxShape* shape = CreateShape(PxBoxGeometry(0.1, 0.1, 0.1));
	auto a = std::chrono::high_resolution_clock::now();
	double actualTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();

	Particle* part = new Particle(pos, v, mass, 0.99, 4, actualTime, shape, _model_particle->getColor());

	particles.push_back(part);

	return part;
}

std::list<Particle*> UniformParticleGenerator::activeParticles()
{
	return listParticles;
}

void UniformParticleGenerator::deleteParticle(Particle* p)
{
	listParticles.remove(p);

}

void UniformParticleGenerator::integrate(float t)
{
	auto a = std::chrono::high_resolution_clock::now();
	double actualTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();

	auto it = listParticles.begin();
	while (it != listParticles.end()) {

		(*it)->integrate(t);

		if (actualTime > (*it)->getStartTime() + (*it)->getLifeTime()) {
			// Eliminar el elemento y avanzar el iterador
			delete* it;
			it = listParticles.erase(it);
		}
		else {
			// Avanzar el iterador
			++it;
		}
	}

}
