#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{

}

ParticleSystem::~ParticleSystem()
{
	auto it = Fireworks.begin();
	while (it != Fireworks.end()) {
		delete (*it);
		it = Fireworks.erase(it);
	}
}

void ParticleSystem::generate(FireworksRules fr)
{

	Fireworks.push_back(new Firework(Vector3{ 0,0,0 }, fr, Fireworks));

}

void ParticleSystem::integrate(float t)
{
	createParticles(t);

	for (RBGaussianParticleGenerator* pg : RBparticleGenerators) {
		if (RBparticles.size() < 100)
		{
			RBParticle* p = pg->generateParticles(RBparticles);
			registry.addRegistry(p, forceGenerators);
		}
	}

	registry.updateForces(t);

	for (Particle* elem : particles)
	{
		elem->integrate(t);
	}

	auto it = particles.begin();
	while (it != particles.end()) {
		if ((*it)->getErrase())
		{
			delete (*it);
			it = particles.erase(it);
		}

		else it++;
	}

	for (Particle* elemt : RBparticles)
	{
		elemt->integrate(t);
	}

	auto ot = RBparticles.begin();
	while (ot != RBparticles.end()) {
		if ((*ot)->getErrase())
		{
			delete (*ot);
			ot = RBparticles.erase(ot);
		}

		else ot++;
	}

}

void ParticleSystem::addForceGenerator(ForceGenerator* forceGenerator)
{
	forceGenerators.push_back(forceGenerator);
}
void ParticleSystem::addGravityForceGenerator(GravityForceGenerator* forceGenerator)
{
	forceGenerators.push_back(forceGenerator);
	gravity = forceGenerator;
}
void ParticleSystem::addWindForceGenerator(WindForceGenerator* forceGenerator)
{
	forceGenerators.push_back(forceGenerator);
	wind = forceGenerator;
}
void ParticleSystem::addWhirlwindForceGenerator(WhirlwindForceGenerator* forceGenerator)
{
	forceGenerators.push_back(forceGenerator);
	whirlWind = forceGenerator;
}

ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name)
{
	for (auto it = particleGenerators.begin(); it != particleGenerators.end(); ++it) {
		if ((*it)->getName() == name) return (*it);
	}
}

void ParticleSystem::addParticleGenerator(ParticleGenerator* generator)
{
	particleGenerators.push_back(generator);
}

void ParticleSystem::addRBParticleGenerator(RBGaussianParticleGenerator* generator)
{

	RBparticleGenerators.push_back(generator);
}

void ParticleSystem::createParticles(double t)
{

	for (ParticleGenerator* pg : particleGenerators) {
		Particle* p = pg->generateParticles(particles,t);
		registry.addRegistry(p, forceGenerators);
	}
}
void ParticleSystem::createRBParticles(double t)
{

	for (RBGaussianParticleGenerator* pg : RBparticleGenerators) {
		RBParticle* p = pg->generateParticles(RBparticles);
		if (p != nullptr)
		{
			registry.addRegistry(p, forceGenerators);
		}
	}
}

void ParticleSystem::explosion(float t) {
	if (expGenerator) {
		expGenerator->start(t);
	}
}

void ParticleSystem::addExplosionGenerator(ExplosionForceGenerator* gen) {
	expGenerator = gen;
}

void ParticleSystem::generateSpringDemo() {

	auto a = std::chrono::high_resolution_clock::now();
	float startTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();
	Particle* p1 = new Particle(Vector3{ -10,50,0 }, Vector3{ 0,0,0 }, 2, 0.8, 500, startTime, Vector4(0.5, 0.5, 0.5, 1));
	Particle* p2 = new Particle(Vector3{ 10,50,0 }, Vector3{ 0,0,0 }, 2, 0.8, 500, startTime, Vector4(0.5, 0, 0.7, 1));
	gen1 = new BungeeForceGenerator(1, 10, p2);
	registry.addRegistry(p1, gen1);
	gen2 = new BungeeForceGenerator(1, 10, p1);
	registry.addRegistry(p2, gen2);
	forceGenerators.push_back(gen1);
	forceGenerators.push_back(gen2);
	particles.push_back(p1);
	particles.push_back(p2);
}
void ParticleSystem::generateAnchoredSpringDemo() {

	auto a = std::chrono::high_resolution_clock::now();
	float startTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();
	AnchoredP = new Particle(Vector3{ 0,0,0 }, Vector3{ 0,0,0 }, 12, 1, 500, startTime, CreateShape(PxSphereGeometry(5.0)), Vector4(0, 1, 0, 1));
	AnchoredP->setSize(5);
	AnchoredSpringForceGenerator* f3 = new AnchoredSpringForceGenerator(1, 10, { 0,0,0 });
	registry.addRegistry(AnchoredP, f3);
	registry.addRegistry(AnchoredP, gravity);
	forceGenerators.push_back(f3);
	particles.push_back(AnchoredP);
}
void ParticleSystem::generateBuoyancyDemo() {
	auto a = std::chrono::high_resolution_clock::now();
	float startTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();
	Particle* p1 = new Particle(Vector3{ 0,50,0 }, Vector3{ 0,0,0 }, 10, 0.8, 500, startTime, Vector4(0, 1, 0, 1));
	Particle* p2 = new Particle(Vector3{ 5,50,5 }, Vector3{ 0,0,0 }, 100, 0.8, 500, startTime, Vector4(0, 1, 0, 1));
	Particle* p3 = new Particle(Vector3{ 5,50,-5 }, Vector3{ 0,0,0 }, 300, 0.8, 500, startTime, Vector4(0, 1, 0, 1));
	Particle* p4 = new Particle(Vector3{ -5,50,5 }, Vector3{ 0,0,0 }, 1000, 0.8, 500, startTime, Vector4(0, 1, 0, 1));
	Particle* p5 = new Particle(Vector3{ -5,50,-5 }, Vector3{ 0,0,0 }, 2000, 0.8, 500, startTime, Vector4(0, 1, 0, 1));
	BuoyancyForceGenerator* f4 = new BuoyancyForceGenerator(1000, Vector3(0, 20, 0));
	forceGenerators.push_back(f4);
	registry.addRegistry(p1, f4);
	registry.addRegistry(p2, f4);
	registry.addRegistry(p3, f4);
	registry.addRegistry(p4, f4);
	registry.addRegistry(p5, f4);
	registry.addRegistry(p1, gravity);
	registry.addRegistry(p2, gravity);
	registry.addRegistry(p3, gravity);
	registry.addRegistry(p4, gravity);
	registry.addRegistry(p5, gravity);
	particles.push_back(p1);
	particles.push_back(p2);
	particles.push_back(p3);
	particles.push_back(p4);
	particles.push_back(p5);
}

void ParticleSystem::changeSpringPower(int k)
{
	if (gen1 != nullptr)
	{
		gen1->setK(k);
	}
	if (gen2 != nullptr)
	{
		gen2->setK(k);
	}

}

void ParticleSystem::anchoredSForce(float f)
{
	AnchoredP->addForce({0, f, 0});
}


