#pragma once

#include"Firework.h"
#include "ParticleGenerator.h"
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
	void addExplosionGenerator(ExplosionForceGenerator* generator);
	void createParticles(double t);
	//void generateExplosion(Vector3 pos, double size);
	void explosion(float t);
	void generateSpringDemo();
	void generateAnchoredSpringDemo();
	void generateBuoyancyDemo();
	void changeSpringPower(int k);
	void anchoredSForce(float f);

protected:
	std::list<Particle*> particles;
	std::list <Firework*> Fireworks;
	std::vector<ForceGenerator*> forceGenerators;  // Lista de fuerzas
	std::list<ParticleGenerator*> particleGenerators;
	ForceRegistry registry;
	ExplosionForceGenerator* expGenerator = nullptr;
	BungeeForceGenerator* gen1 = nullptr;
	BungeeForceGenerator* gen2 = nullptr;
	GravityForceGenerator* gravity;
	WindForceGenerator* wind;
	WhirlwindForceGenerator* whirlWind;
	Particle* AnchoredP = nullptr;
};

