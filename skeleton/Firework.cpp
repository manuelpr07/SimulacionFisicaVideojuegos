#include "Firework.h"

Firework::Firework(Vector3 p, FireworksRules frw, std::list <Firework*>& listPart) : Particle(p, frw.vel, frw.colour, frw.damping, frw.lifeTime)
{
	listParticles = &listPart;
	fr = frw;
}

Firework::~Firework()
{
	explode();
	Particle::~Particle();

}

void Firework::integrate(float t)
{
	Particle::integrate(t);

}

void Firework::explode()
{

	FireworksRules newFr;
	bool spawn = true;
	if (fr.id == Fr10.id)
		newFr = Fr11;
	else if (fr.id == Fr11.id)
		newFr = Fr12;
	else if (fr.id == Fr12.id)
		spawn = false;

	if (spawn)
	{
		for (int i = 0; i < fr.numParticles; i++)
		{
			Vector3 newVel;
			std::mt19937 mt(std::random_device{}());
			//calculo de la nueva velocidad
			std::normal_distribution<double> distributionVX(newFr.vel.x, 5);
			newVel.x = distributionVX(mt);

			std::normal_distribution<double> distributionVY(newFr.vel.y, 10);
			newVel.y = distributionVY(mt);

			std::normal_distribution<double> distributionVZ(newFr.vel.z, 10);
			newVel.z = distributionVZ(mt);
			newFr.vel = { newVel.x , newVel.y, newVel.z };
			listParticles->push_back(new Firework(getPos().p, newFr, *listParticles));
		}
	}
}


