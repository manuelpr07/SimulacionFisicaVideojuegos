#include "Firework.h"

Firework::Firework(Vector3 p, FireworksRules f, std::list <Firework*>& listPart): Particle( p,  f.vel, f.colour, f.damping,  f.lifeTime)
{
	listParticles = &listPart;
	fr = f;
}

Firework::~Firework()
{
	if (fr.level < 2)
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
	if (fr.level == 0)
	{
		newFr.colour = { 1, 0.5, 0, 1 };
		newFr.numParticles = 10;
		newFr.vel = { 0,10,0 };
		newFr.damping = 1;
		newFr.lifeTime = 2;
		newFr.level = 1;

	}
	else if (fr.level == 1)
	{
		newFr.colour = { 1, 1, 0, 1 };
		newFr.numParticles = 5;
		newFr.vel = { 0,5,0 };
		newFr.damping = 1;
		newFr.lifeTime = 4;
		newFr.level = 2;

	}
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


