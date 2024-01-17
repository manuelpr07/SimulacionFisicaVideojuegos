#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(PxPhysics* phys, PxScene* scene)
{
	gPhysics = phys;
	gScene = scene;


	boton = new Boton({ 0,10,0 }, { 0.7, 0, 0, 1 });

	//generador rigido uniforme (disparo de la camara)new Particle(pos, v, mass, 0.99, 4, actualTime, shape, _model_particle->getColor());
	Particle* part = new Particle({ 1000, 50, 1000 }, { 0, -15, 0 }, 5, 0.99, 5, 0, CreateShape(PxBoxGeometry(1, 1, 1)), { 0.4, 0.1, 0.6, 1 });
	uniformGenerator = new UniformParticleGenerator(Vector3{ 5, 5, 5 }, part); ///cambiar el UniformParticleGenerator para que funcione
	delete part;

	scene = 0;
}

ParticleSystem::~ParticleSystem()
{
	clearScene();

}

void ParticleSystem::generate(FireworksRules fr)
{

	Fireworks.push_back(new Firework(Vector3{ 0,0,0 }, fr, Fireworks));

}

void ParticleSystem::integrate(float t)
{
	createParticles(t);



	registry.updateForces(t);

	for (Particle* elem : particles)
	{
		elem->integrate(t);
	}

	auto it = particles.begin();
	while (it != particles.end()) {
		if ((std::abs((*it)->getPos().p.x) > tamanoCaja.x / 2 || std::abs((*it)->getPos().p.y) > tamanoCaja.y / 2 || std::abs((*it)->getPos().p.z) > tamanoCaja.z / 2))
			(*it)->setErrase();
		if ((*it)->getErrase())
		{
			registry.registry.erase(*it);
			delete (*it);
			it = particles.erase(it);
		}

		else it++;
	}

	for (Particle* elem : shootingParticles)
	{
		elem->integrate(t);
		boton->DetectarColision(elem);
	}

	auto ut = shootingParticles.begin();
	while (ut != shootingParticles.end()) {
		if ((std::abs((*ut)->getPos().p.x) > tamanoCaja.x / 2 || std::abs((*ut)->getPos().p.y) > tamanoCaja.y / 2 || std::abs((*ut)->getPos().p.z) > tamanoCaja.z / 2))
			(*ut)->setErrase();
		if ((*ut)->getErrase())
		{
			registry.registry.erase(*ut);
			delete (*ut);
			ut = shootingParticles.erase(ut);
		}

		else ut++;
	}

	for (Firework* elem : Fireworks)
	{
		elem->integrate(t);
	}
	auto it1 = Fireworks.begin();
	while (it1 != Fireworks.end()) {
		if ((*it1)->getErrase())
		{
			registry.registry.erase(*it1);
			delete (*it1);
			it1 = Fireworks.erase(it1);
		}

		else it1++;
	}

	for (Particle* elemt : RBparticles)
	{
		elemt->integrate(t);
	}

	auto ot = RBparticles.begin();
	while (ot != RBparticles.end()) {
		if ((std::abs((*ot)->getPos().p.x) > tamanoCaja.x / 2 || std::abs((*ot)->getPos().p.y) > tamanoCaja.y / 2 || std::abs((*ot)->getPos().p.z) > tamanoCaja.z / 2))
			(*ot)->setErrase();
		if ((*ot)->getErrase())
		{
			registry.registry.erase(*ot);
			delete (*ot);
			ot = RBparticles.erase(ot);
		}

		else ot++;
	}

	boton->integrate(t);
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

void ParticleSystem::addRBGausianParticleGenerator(RBGaussianParticleGenerator* generator)
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
	if (anchored == nullptr)
	{
		auto a = std::chrono::high_resolution_clock::now();
		float startTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();
		AnchoredP = new Particle(Vector3{ 0,0,0 }, Vector3{ 0,0,0 }, 12, 1, 500, startTime, CreateShape(PxSphereGeometry(5.0)), Vector4(0, 1, 0, 1));
		AnchoredP->setSize(5);
		anchored = new AnchoredSpringForceGenerator(1, 10, { 0,0,0 });
		registry.addRegistry(AnchoredP, anchored);
		registry.addRegistry(AnchoredP, gravity);
		forceGenerators.push_back(anchored);
		particles.push_back(AnchoredP);
	}
}
void ParticleSystem::generateBuoyancyDemo() {
	if (buoyancy == nullptr)
	{
		auto a = std::chrono::high_resolution_clock::now();
		float startTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();
		Particle* p1 = new Particle(Vector3{ 0,50,0 }, Vector3{ 0,0,0 }, 10, 0.8, 500, startTime, Vector4(0, 1, 0, 1));
		Particle* p2 = new Particle(Vector3{ 5,50,5 }, Vector3{ 0,0,0 }, 100, 0.8, 500, startTime, Vector4(0, 1, 0, 1));
		Particle* p3 = new Particle(Vector3{ 5,50,-5 }, Vector3{ 0,0,0 }, 300, 0.8, 500, startTime, Vector4(0, 1, 0, 1));
		Particle* p4 = new Particle(Vector3{ -5,50,5 }, Vector3{ 0,0,0 }, 1000, 0.8, 500, startTime, Vector4(0, 1, 0, 1));
		Particle* p5 = new Particle(Vector3{ -5,50,-5 }, Vector3{ 0,0,0 }, 2000, 0.8, 500, startTime, Vector4(0, 1, 0, 1));
		buoyancy = new BuoyancyForceGenerator(1000, Vector3(0, 20, 0));
		forceGenerators.push_back(buoyancy);
		registry.addRegistry(p1, buoyancy);
		registry.addRegistry(p2, buoyancy);
		registry.addRegistry(p3, buoyancy);
		registry.addRegistry(p4, buoyancy);
		registry.addRegistry(p5, buoyancy);
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

void ParticleSystem::createRB()
{
	for (RBGaussianParticleGenerator* pg : RBparticleGenerators) {
		while (RBparticles.size() < 100) {

			RBParticle* p = pg->generateParticles(RBparticles);
			registry.addRegistry(p, forceGenerators);
		}
	}
}

void ParticleSystem::clearScene()
{

	registry.clearRegistry();

	if (!Fireworks.empty()) {
		auto it = Fireworks.begin();
		while (it != Fireworks.end()) {
			(*it)->setErrase();
			it++;
		}
	}
	if (!particles.empty()) {
		auto it1 = particles.begin();
		while (it1 != particles.end()) {
			registry.registry.erase(*it1);
			(*it1)->setErrase();
			it1++;
		}
	}
	if (!RBparticles.empty()) {
		auto it2 = RBparticles.begin();
		while (it2 != RBparticles.end()) {
			registry.registry.erase(*it2);
			(*it2)->setErrase();
			it2++;
		}
	}
	if (!shootingParticles.empty()) {
		auto it3 = shootingParticles.begin();
		while (it3 != shootingParticles.end()) {
			registry.registry.erase(*it3);
			(*it3)->setErrase();
			it3++;
		}
	}
	if (gen1 != nullptr)
	{
		gen1 = nullptr;
	}
	if (gen2 != nullptr)
	{
		gen2 = nullptr;
	}
	if (anchored != nullptr)
	{
		anchored->clear();
		anchored = nullptr;
	}
	if (wind != nullptr)
	{
		wind = nullptr;
	}
	if (whirlWind != nullptr)
	{
		whirlWind = nullptr;
	}
	if (buoyancy != nullptr)
	{
		buoyancy->clear();
		buoyancy = nullptr;
	}

}


void ParticleSystem::shootProjectile(const PxTransform& camera)
{
	Vector3 pos = Vector3(camera.p.x, camera.p.y, camera.p.z);
	PxVec3 direction = camera.q.getBasisVector2() * -1;

	float speed = 100.0f;;
	Vector3 v = Vector3(direction.x * speed, direction.y * speed, direction.z * speed);;
	double mass = 30;


	Vector3 acceleration = Vector3(0, -9.81, 0);

	Particle* p = uniformGenerator->generate(shootingParticles, pos, v, mass);

	registry.addRegistry(p, gravity);
}

void ParticleSystem::changeScene()
{
	if (scene < 100)
	{
		clearScene();
		scene++;

		switch (scene)
		{
			case '1': {

			}; break;
			case '2': {

			}; break;
		}

	}
}



