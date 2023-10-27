#include "gaussianParticleGenerator.h"

gaussianParticleGenerator::gaussianParticleGenerator(Vector3 dPos, Vector3 dVel, Particle* m) : ParticleGenerator(m)
{
	desvVel = dVel;
	desvPos = dPos;
	area = { 50, 100, 50 };

}
gaussianParticleGenerator::~gaussianParticleGenerator()
{

	auto it = listParticles.begin();
	while (it != listParticles.end()) {
		it = listParticles.erase(it);
	}

}

void gaussianParticleGenerator::generateParticles(double t)
{
	Vector3 newPos;
	Vector3 newVel;

	std::mt19937 mt(std::random_device{}());

	//calculo de la nueva posicion
	std::normal_distribution<double> distributionPX(_model_particle->getPos().x, desvPos.x);
	newPos.x = distributionPX(mt);

	std::normal_distribution<double> distributionPY(_model_particle->getPos().y, desvPos.y);
	newPos.y = distributionPY(mt);

	std::normal_distribution<double> distributionPZ(_model_particle->getPos().z, desvPos.z);
	newPos.z = distributionPZ(mt);

	//calculo de la nueva velocidad
	std::normal_distribution<double> distributionVX(_model_particle->getVel().x, desvPos.x);
	newVel.x = distributionVX(mt);

	std::normal_distribution<double> distributionVY(_model_particle->getVel().y, desvPos.y);
	newVel.y = distributionVY(mt);

	std::normal_distribution<double> distributionVZ(_model_particle->getVel().z, desvPos.z);
	newVel.z = distributionVZ(mt);
	auto a = std::chrono::high_resolution_clock::now();
	double startTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();
	Particle* part = new Particle(newPos, newVel, 12, _model_particle->getDamping(), _model_particle->getLifeTime(), startTime);

	listParticles.push_back(part);

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
