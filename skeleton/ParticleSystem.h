#pragma once

#include"Firework.h"
#include "ParticleGenerator.h"
#include "RBGaussianParticleGenerator.h"
#include "ForceGenerator.h"
#include "ForceRegistry.h"

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();
	void generate(FireworksRules fr);
	void integrate(float t);

	void addForceGenerator(ForceGenerator* forceGenerator);
	void addGravityForceGenerator(GravityForceGenerator* forceGenerator);
	void addWindForceGenerator(WindForceGenerator* forceGenerator);
	void addWhirlwindForceGenerator(WhirlwindForceGenerator* forceGenerator);

	ParticleGenerator* getParticleGenerator(std::string name);
	void addParticleGenerator(ParticleGenerator* generator);
	void addRBParticleGenerator(RBGaussianParticleGenerator* generator);
	void addExplosionGenerator(ExplosionForceGenerator* generator);
	void createParticles(double t);
	void createRBParticles(double t);
	//void generateExplosion(Vector3 pos, double size);
	void explosion(float t);
	void generateSpringDemo();
	void generateAnchoredSpringDemo();
	void generateBuoyancyDemo();
	void changeSpringPower(int k);
	void anchoredSForce(float f);
	void createRB();
	void clearScene();

protected:
	std::list<Particle*> particles;
	std::list<RBParticle*> RBparticles;
	std::list <Firework*> Fireworks;
	std::vector<ForceGenerator*> forceGenerators;  // Lista de fuerzas
	std::list<ParticleGenerator*> particleGenerators;
	std::list<RBGaussianParticleGenerator*> RBparticleGenerators;
	ForceRegistry registry;
	ExplosionForceGenerator* expGenerator = nullptr;
	BungeeForceGenerator* gen1 = nullptr;
	BungeeForceGenerator* gen2 = nullptr;
	GravityForceGenerator* gravity = nullptr;
	WindForceGenerator* wind = nullptr;
	WhirlwindForceGenerator* whirlWind = nullptr;
	BuoyancyForceGenerator* buoyancy = nullptr;
	AnchoredSpringForceGenerator* anchored = nullptr;
	Particle* AnchoredP = nullptr;
};

