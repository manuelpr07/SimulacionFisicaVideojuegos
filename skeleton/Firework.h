#include "Particle.h"
#include <list>
#include <string>
#include <iostream>
#include <random>
using namespace physx;

struct FireworksRules {
	Vector4 colour;
	int numParticles;
	Vector3 vel;
	float damping;
	float lifeTime;
	int level;
};

class Firework : public Particle
{
public:
	Firework(Vector3 p, FireworksRules f, std::list <Firework*>& listPart);
	~Firework();
	void integrate(float t);
	void explode();


private:
	float start;
	Vector3 pos;
	FireworksRules fr;
	std::list <Firework*>* listParticles;

	//std::mt19937 _mt;
	//std::uniform_real_distribution<double> _u{ -1,1 };
	//std::string _name;
};