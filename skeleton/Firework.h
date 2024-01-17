#pragma once
#include "Particle.h"
#include <list>
#include <string>
#include <iostream>
#include <random>

using namespace physx;


struct FireworksRules {
	int id;
	Vector4 colour;
	int numParticles;
	Vector3 vel;
	float damping;
	float lifeTime;
};


class Firework : public Particle
{

public:



	Firework(Vector3 p, FireworksRules fr, std::list <Firework*>& listPart);
	~Firework();
	void integrate(float t);
	void explode();



	FireworksRules Fr10 = { 0, { 1, 0, 0, 1 }, 10, { 0, 30, 0 }, 1, 3 };
	FireworksRules Fr11 = { 1, { 1, 0.5, 0, 1 }, 10, { 0, 10, 0 }, 1, 2 };
	FireworksRules Fr12 = { 2, { 1, 1, 0, 1 }, 5, { 0, 5, 0 }, 1, 1.5 };


private:
	float start;
	Vector3 pos;
	FireworksRules fr;
	std::list <Firework*>* listParticles;


	//std::mt19937 _mt;
	//std::uniform_real_distribution<double> _u{ -1,1 };
	//std::string _name;
};