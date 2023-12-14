#include "gaussianParticleGenerator.h"

gaussianParticleGenerator::gaussianParticleGenerator(Vector3 dPos, Vector3 dVel, Particle* m, std::string name) : ParticleGenerator(m)
{
	desvVel = dVel;
	desvPos = dPos;
	area = { 50, 100, 50 };
	_name = name;

}
gaussianParticleGenerator::~gaussianParticleGenerator()
{

	auto it = listParticles.begin();
	while (it != listParticles.end()) {
		it = listParticles.erase(it);
	}

}

Particle* gaussianParticleGenerator::generateParticles(std::list<Particle*>& particles, float t)
{
	Vector3 newPos;
	Vector3 newVel;

	std::mt19937 mt(std::random_device{}());

	//calculo de la nueva posicion
	std::normal_distribution<double> distributionPX(_model_particle->getPos().p.x, desvPos.x);
	newPos.x = distributionPX(mt);

	std::normal_distribution<double> distributionPY(_model_particle->getPos().p.y, desvPos.y);
	newPos.y = distributionPY(mt);

	std::normal_distribution<double> distributionPZ(_model_particle->getPos().p.z, desvPos.z);
	newPos.z = distributionPZ(mt);

	//calculo de la nueva velocidad
	std::normal_distribution<double> distributionVX(_model_particle->getVel().x, desvVel.x);
	newVel.x = distributionVX(mt);

	std::normal_distribution<double> distributionVY(_model_particle->getVel().y, desvVel.y);
	newVel.y = distributionVY(mt);

	std::normal_distribution<double> distributionVZ(_model_particle->getVel().z, desvVel.z);
	newVel.z = distributionVZ(mt);

	auto a = std::chrono::high_resolution_clock::now();
	float startTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();
	Particle* part = new Particle(newPos, newVel, _model_particle->getMass(), _model_particle->getDamping(), _model_particle->getLifeTime(), startTime, _model_particle->getColor());

	particles.push_back(part);
	return part;
}

std::list<Particle*> gaussianParticleGenerator::activeParticles()
{
	return listParticles;
}

void gaussianParticleGenerator::deleteParticle(Particle* p)
{
	listParticles.remove(p);

}

void gaussianParticleGenerator::integrate(float t)
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
