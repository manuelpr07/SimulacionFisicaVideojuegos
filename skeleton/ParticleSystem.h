#pragma once

#include"Firework.h"
#include "ParticleGenerator.h"
#include "RBGaussianParticleGenerator.h"
#include "RBUniformParticleGenerator.h"
#include "ForceGenerator.h"
#include "ForceRegistry.h"
#include "boton.h"
#include "UniformParticleGenerator.h"

class ParticleSystem
{
public:
	ParticleSystem(PxPhysics* phys, PxScene* scene);
	~ParticleSystem();
	void generate(FireworksRules fr);
	void integrate(float t);

	void addForceGenerator(ForceGenerator* forceGenerator);
	void addGravityForceGenerator(GravityForceGenerator* forceGenerator);
	void addWindForceGenerator(WindForceGenerator* forceGenerator);
	void addWhirlwindForceGenerator(WhirlwindForceGenerator* forceGenerator);

	ParticleGenerator* getParticleGenerator(std::string name);
	void addParticleGenerator(ParticleGenerator* generator);
	void addRBGausianParticleGenerator(RBGaussianParticleGenerator* generator);
	void addRBUniformParticleGenerator(RBGaussianParticleGenerator* generator);
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
	void shootProjectile(const PxTransform& camera);
	void changeScene();
	void scene1();
protected:
	const Vector3 tamanoCaja = {200,200,200};
	std::list<Particle*> particles;
	std::list<Particle*> shootingParticles;
	std::list<RBParticle*> RBparticles;
	std::list <Firework*> Fireworks;
	std::vector<ForceGenerator*> forceGenerators;  // Lista de fuerzas
	std::list<ParticleGenerator*> particleGenerators;
	std::list<RBGaussianParticleGenerator*> RBparticleGenerators;
	UniformParticleGenerator* uniformGenerator;
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
	Boton* boton = nullptr;

	PxPhysics* gPhysics;
	PxScene* gScene;

	int scene;
};

