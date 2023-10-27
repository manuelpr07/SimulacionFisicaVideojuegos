#include "UniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator( Vector3 posW, Vector3 velW, Particle* m): ParticleGenerator(m)
{
	velWidth = velW;
	posWidth = posW;

}
UniformParticleGenerator::~UniformParticleGenerator()
{

	for (Particle* part : activeParticles())
		delete part;

}

void UniformParticleGenerator::generateParticles(double t)
{
	Vector3 newPos;
	Vector3 newVel;

	//calculo de la nueva posicion
	double random_number = _u(_mt);
	newPos.x = _model_particle->getPos().x + posWidth.x * random_number;

	random_number = _u(_mt);
	newPos.y = _model_particle->getPos().y + posWidth.y * random_number;

	random_number = _u(_mt);
	newPos.z = _model_particle->getPos().z + posWidth.z * random_number;

	//calculo de la nueva velocidad
	random_number = _u(_mt);
	newVel.x = _model_particle->getVel().x + velWidth.x * random_number;

	random_number = _u(_mt);
	newVel.y = _model_particle->getVel().y + velWidth.y * random_number;
	
	random_number = _u(_mt);
	newVel.z = _model_particle->getVel().z + velWidth.z * random_number;


	Particle* part = new Particle(newPos, newVel, 12, _model_particle->getDamping(), _model_particle->getLifeTime(), t);

	listParticles.push_back(part);

	 

	//(Vector3 pos, Vector3 realVel, int realMass, float damp, double lifeT)



		//pose = PxTransform(pos);
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
