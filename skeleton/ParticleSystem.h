#pragma once

#include"Firework.h"
#include "ParticleGenerator.h"
#include "RBGaussianParticleGenerator.h"
#include "RBUniformParticleGenerator.h"
#include "ForceGenerator.h"
#include "ForceRegistry.h"
#include "boton.h"
#include "UniformParticleGenerator.h"
#include "gaussianParticleGenerator.h"

class ParticleSystem
{
public:
	ParticleSystem(PxPhysics* phys, PxScene* Scene);
	~ParticleSystem();
	void generate(FireworksRules fr, Vector3 pos);
	void integrate(float t);

	void addForceGenerator(ForceGenerator* forceGenerator);
	void addGravityForceGenerator(GravityForceGenerator* forceGenerator);
	void addWindForceGenerator(WindForceGenerator* forceGenerator);
	void addWhirlwindForceGenerator(WhirlwindForceGenerator* forceGenerator);

	ParticleGenerator* getParticleGenerator(std::string name);
	void addParticleGenerator(ParticleGenerator* generator);
	void addRBGausianParticleGenerator(RBGaussianParticleGenerator* generator);
	void addExplosionGenerator(ExplosionForceGenerator* generator);
	void createParticles(double t);
	void createRBParticles();
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
	void sceneIntegrate();
	void scene1();
	void scene2();
	void scene3();
	void scene4();
	void scene5();
	void scene6();
	void scene7();
	void scene8();
	void scene9();
	void scene10();
protected:
	const Vector3 tamanoCaja = {250,250,250};
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

	bool scene1Intgr = false;
	bool scene8Intgr = false;
	int scene;

	FireworksRules Fr10 = { 0, { 1, 0, 0, 1 }, 10, { 0, 30, 0 }, 1, 3 };
	FireworksRules Fr11 = { 1, { 1, 0.5, 0, 1 }, 10, { 0, 10, 0 }, 1, 2 };
	FireworksRules Fr12 = { 2, { 1, 1, 0, 1 }, 5, { 0, 5, 0 }, 1, 1.5 };

	double frActivationTime;
	double frBuffer = 0.8;
	bool frActivado;

	double rbBuffer = 0.05;
};

