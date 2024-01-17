#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(PxPhysics* phys, PxScene* Scene)
{
	gPhysics = phys;
	gScene = Scene;

	boton = new Boton({ 40,40,40 }, { 0.7, 0, 0, 1 });

	//generador rigido uniforme (disparo de la camara)new Particle(pos, v, mass, 0.99, 4, actualTime, shape, _model_particle->getColor());
	Particle* part = new Particle({ 1000, 50, 1000 }, { 0, -15, 0 }, 5, 0.99, 5, 0, CreateShape(PxBoxGeometry(1, 1, 1)), { 0.4, 0.1, 0.6, 1 });
	uniformGenerator = new UniformParticleGenerator(Vector3{ 5, 5, 5 }, part); ///cambiar el UniformParticleGenerator para que funcione
	delete part;

	scene = 0;
}

ParticleSystem::~ParticleSystem()
{
	auto it = particles.begin();
	while (it != particles.end()) {
		registry.registry.erase(*it);
		delete (*it);
		it = particles.erase(it);
	}

	auto ut = shootingParticles.begin();
	while (ut != shootingParticles.end()) {
		registry.registry.erase(*ut);
		delete (*ut);
		ut = shootingParticles.erase(ut);
	}


	auto it1 = Fireworks.begin();
	while (it1 != Fireworks.end()) {
		registry.registry.erase(*it1);
		delete (*it1);
		it1 = Fireworks.erase(it1);
	}


	auto ot = RBparticles.begin();
	while (ot != RBparticles.end()) {
		registry.registry.erase(*ot);
		delete (*ot);
		ot = RBparticles.erase(ot);
	}

}

void ParticleSystem::generate(FireworksRules fr, Vector3 pos)
{

	Fireworks.push_back(new Firework(pos, fr, Fireworks));

}

void ParticleSystem::integrate(float t)
{
	createParticles(t);

	sceneIntegrate();

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
		if(boton->DetectarColision(elem))
			changeScene();
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

	//for (Particle* elemt : RBparticles)
	//{
	//	elemt->integrate(t);
	//}

	//auto ot = RBparticles.begin();
	//while (ot != RBparticles.end()) {
	//	if ((std::abs((*ot)->getPos().p.x) > tamanoCaja.x / 2 || std::abs((*ot)->getPos().p.y) > tamanoCaja.y / 2 || std::abs((*ot)->getPos().p.z) > tamanoCaja.z / 2))
	//		(*ot)->setErrase();
	//	if ((*ot)->getErrase())
	//	{
	//		registry.registry.erase(*ot);
	//		delete (*ot);
	//		ot = RBparticles.erase(ot);
	//	}

	//	else ot++;
	//}

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

	if (RBparticles.size() < 500) {
		for (RBGaussianParticleGenerator* pg : RBparticleGenerators) {

			RBParticle* p = pg->generateParticles(RBparticles);
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

	if (!particleGenerators.empty()) {
		auto it4 = particleGenerators.begin();
		while (it4 != particleGenerators.end()) {
			it4 = particleGenerators.erase(it4);
		}
	}

	if (!forceGenerators.empty()) {
		auto it5 = forceGenerators.begin();
		while (it5 != forceGenerators.end()) {
			if ((*it5) != gravity)
				it5 = forceGenerators.erase(it5);
			else it5++;
		}
	}
	
	if (!RBparticles.empty()) {
		auto it6 = RBparticles.begin();
		while (it6 != RBparticles.end()) {
			it6 = RBparticles.erase(it6);
		}
	}
	if (!RBparticleGenerators.empty()) {
		auto it7 = RBparticleGenerators.begin();
		while (it7 != RBparticleGenerators.end()) {
			it7 = RBparticleGenerators.erase(it7);
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
		scene++;

		switch (scene)
		{
			case 1: {
				scene1();
			}; break;
			case 2: {
				scene1Intgr = false;
				scene2();
			}; break;
			case 3: {
				scene3();
			}; break;
			case 4: {
				scene4();
			}; break;
			case 5: {
				scene5();
			}; break;
			case 6: {
				scene6();
			}; break;
			case 7: {
				scene7();
			}; break;
			case 8: {
				scene8();
			}; break;
			case 9: {
				scene9();
			}; break;
			case 10: {
				scene10();
			}; break;
		}

	}
}

void ParticleSystem::sceneIntegrate()
{
	if (scene1Intgr)
	{
		auto a = std::chrono::high_resolution_clock::now();
		double actualTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();

		if (actualTime > frActivationTime + frBuffer) {
			frActivado = false;
		}
		if (!frActivado)
		{
			generate(Fr10, Vector3{ 90,0,90 });
			generate(Fr10, Vector3{ -90,0,90 });
			generate(Fr10, Vector3{ 90,0,-90 });
			generate(Fr10, Vector3{ -90,0,-90 });
			frActivationTime = actualTime;
			frActivado = true;
		}
	}
	if (scene8Intgr)
	{
		auto a = std::chrono::high_resolution_clock::now();
		double actualTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();

		if (actualTime > frActivationTime + rbBuffer) {
			frActivado = false;
		}
		if (!frActivado)
		{
			createRBParticles(actualTime);
			frActivationTime = actualTime;
			frActivado = true;
		}
	}
}

void ParticleSystem::scene1()
{
	scene1Intgr = true;

	Particle* part = new Particle(Vector3{70,-5,0 }, Vector3{ 0,30,0 }, 12, 0.9, 4, Vector4(0, 0.2, 0.8, 1));
	addParticleGenerator(new gaussianParticleGenerator(Vector3{ 0, 0, 0 }, Vector3{ 5, 3, 5 }, part, "fuente1"));
	particles.push_back(part);
	part->setErrase();

	Particle* part1 = new Particle(Vector3{-70,-5,0 }, Vector3{ 0,30,0 }, 12, 0.9, 4, Vector4(0, 0.2, 0.8, 1));
	addParticleGenerator(new gaussianParticleGenerator(Vector3{ 0, 0, 0 }, Vector3{ 5, 3, 5 }, part1, "fuente2"));
	particles.push_back(part1);
	part1->setErrase();

	Particle* part2 = new Particle(Vector3{ 0,-5,70 }, Vector3{ 0,30,0 }, 12, 0.9, 4, Vector4(0, 0.2, 0.8, 1));
	addParticleGenerator(new gaussianParticleGenerator(Vector3{ 0, 0, 0 }, Vector3{ 5, 3, 5 }, part2, "fuente3"));
	particles.push_back(part2);
	part2->setErrase();

	Particle* part3 = new Particle(Vector3{ 0,-5,-70 }, Vector3{ 0,30,0 }, 12, 0.9, 4, Vector4(0, 0.2, 0.8, 1));
	addParticleGenerator(new gaussianParticleGenerator(Vector3{ 0, 0, 0 }, Vector3{ 5, 3, 5 }, part3, "fuente4"));
	particles.push_back(part3);
	part3->setErrase();

}

void ParticleSystem::scene2()
{
	clearScene();

	Particle* part = new Particle(Vector3{ 80,-5,0 }, Vector3{ 0,30,0 }, 30, 0.9, 6, Vector4(0, 0.2, 0.8, 1));
	addParticleGenerator(new gaussianParticleGenerator(Vector3{ 0, 0, 0 }, Vector3{ 5, 3, 5 }, part, "fuente1"));
	particles.push_back(part);
	part->setErrase();

	Particle* part1 = new Particle(Vector3{ -80,-5,0 }, Vector3{ 0,30,0 }, 30, 0.9, 6, Vector4(0, 0.2, 0.8, 1));
	addParticleGenerator(new gaussianParticleGenerator(Vector3{ 0, 0, 0 }, Vector3{ 5, 3, 5 }, part1, "fuente2"));
	particles.push_back(part1);
	part1->setErrase();

	Particle* part2 = new Particle(Vector3{ 0,-5,80 }, Vector3{ 0,30,0 }, 30, 0.9, 6, Vector4(0, 0.2, 0.8, 1));
	addParticleGenerator(new gaussianParticleGenerator(Vector3{ 0, 0, 0 }, Vector3{ 5, 3, 5 }, part2, "fuente3"));
	particles.push_back(part2);
	part2->setErrase();

	Particle* part3 = new Particle(Vector3{ 0,-5,-80 }, Vector3{ 0,30,0 }, 30, 0.9, 6, Vector4(0, 0.2, 0.8, 1));
	addParticleGenerator(new gaussianParticleGenerator(Vector3{ 0, 0, 0 }, Vector3{ 5, 3, 5 }, part3, "fuente4"));
	particles.push_back(part3);
	part3->setErrase();

	Particle* part4 = new Particle(Vector3{ 0,-5,-0 }, Vector3{ 0,40,0 }, 80, 0.8, 6, Vector4(0.8, 0.2, 0, 1));
	addParticleGenerator(new gaussianParticleGenerator(Vector3{ 0, 0, 0 }, Vector3{ 5, 3, 5 }, part4, "fuente5"));
	particles.push_back(part4);
	part4->setErrase();
}

void ParticleSystem::scene3()
{
	addWindForceGenerator(new WindForceGenerator(Vector3{ -10, 3, 0 }, 10));
}

void ParticleSystem::scene4()
{
	if (wind != nullptr)
	{
		wind = nullptr;
	}
	addWindForceGenerator(new WindForceGenerator(Vector3{ -20, 10, 0 }, 30));
}

void ParticleSystem::scene5()
{
	if (wind != nullptr)
	{
		wind = nullptr;
	}
	addWhirlwindForceGenerator(new WhirlwindForceGenerator(85, Vector3{ 0, 0, 0 }, 3, 6));
}

void ParticleSystem::scene6()
{
	clearScene();
	//1
	auto a = std::chrono::high_resolution_clock::now();
	float startTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();
	Particle* p1 = new Particle(Vector3{ 90,10,30 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0.5, 0, 0.7, 1));
	Particle* p2 = new Particle(Vector3{ -90,10,30 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0, 0.7, 0, 1));
	BungeeForceGenerator* bf11 = new BungeeForceGenerator(5, 10, p2);
	registry.addRegistry(p1, bf11);
	BungeeForceGenerator* bf12 = new BungeeForceGenerator(5, 10, p1);
	registry.addRegistry(p2, bf12);
	forceGenerators.push_back(bf11);
	forceGenerators.push_back(bf12);
	particles.push_back(p1);
	particles.push_back(p2);

	//2
	Particle* p3 = new Particle(Vector3{ 90,10,-30 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0, 0.7, 0, 1));
	Particle* p4 = new Particle(Vector3{ -90,10,-30 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0.5, 0, 0.7, 1));
	BungeeForceGenerator* bf21 = new BungeeForceGenerator(5, 10, p4);
	registry.addRegistry(p3, bf21);
	BungeeForceGenerator* bf22 = new BungeeForceGenerator(5, 10, p3);
	registry.addRegistry(p4, bf22);
	forceGenerators.push_back(bf21);
	forceGenerators.push_back(bf22);
	particles.push_back(p3);
	particles.push_back(p4);

	//3
	Particle* p5 = new Particle(Vector3{ 90,10, 90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0, 0.7, 0, 1));
	Particle* p6 = new Particle(Vector3{ -90,10,90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0.5, 0, 0.7, 1));
	BungeeForceGenerator* bf31 = new BungeeForceGenerator(1, 10, p6);
	registry.addRegistry(p5, bf31);
	BungeeForceGenerator* bf32 = new BungeeForceGenerator(1, 10, p5);
	registry.addRegistry(p6, bf32);
	forceGenerators.push_back(bf31);
	forceGenerators.push_back(bf32);
	particles.push_back(p5);
	particles.push_back(p6);

	//4																
	Particle* p7 = new Particle(Vector3{ 90,10, -90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0.5, 0, 0.7, 1));
	Particle* p8 = new Particle(Vector3{ -90,10,-90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0, 0.7, 0, 1));
	BungeeForceGenerator* bf41 = new BungeeForceGenerator(1, 10, p8);
	registry.addRegistry(p7, bf41);
	BungeeForceGenerator* bf42 = new BungeeForceGenerator(1, 10, p7);
	registry.addRegistry(p8, bf42);
	forceGenerators.push_back(bf41);
	forceGenerators.push_back(bf42);
	particles.push_back(p7);
	particles.push_back(p8);

	//5
	Particle* p9 = new Particle(Vector3{ 90,10,90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0.5, 0, 0.7, 1));
	Particle* p10 = new Particle(Vector3{ 90,10,-90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0, 0.7, 0, 1));
	BungeeForceGenerator* bf51 = new BungeeForceGenerator(1, 10, p10);
	registry.addRegistry(p9, bf51);
	BungeeForceGenerator* bf52 = new BungeeForceGenerator(1, 10, p9); 
	registry.addRegistry(p10, bf52);
	forceGenerators.push_back(bf51);
	forceGenerators.push_back(bf52);
	particles.push_back(p9);
	particles.push_back(p10);

	//6
	Particle* p11 = new Particle(Vector3{ 30,10,-90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0, 0.7, 0, 1));
	Particle* p12 = new Particle(Vector3{ 30,10,90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0.5, 0, 0.7, 1));
	BungeeForceGenerator* bf61 = new BungeeForceGenerator(5, 10, p12);
	registry.addRegistry(p11, bf61);
	BungeeForceGenerator* bf62 = new BungeeForceGenerator(5, 10, p11);
	registry.addRegistry(p12, bf62);
	forceGenerators.push_back(bf61);
	forceGenerators.push_back(bf62);
	particles.push_back(p11);
	particles.push_back(p12);

	//7
	Particle* p13 = new Particle(Vector3{ -30,10, -90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0.5, 0, 0.7, 1));
	Particle* p14 = new Particle(Vector3{ -30,10,90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0, 0.7, 0, 1));
	BungeeForceGenerator* bf71 = new BungeeForceGenerator(5, 10, p14);
	registry.addRegistry(p13, bf71);
	BungeeForceGenerator* bf72 = new BungeeForceGenerator(5, 10, p13);
	registry.addRegistry(p14, bf72);
	forceGenerators.push_back(bf71);
	forceGenerators.push_back(bf72);
	particles.push_back(p13);
	particles.push_back(p14);

	//8																
	Particle* p15 = new Particle(Vector3{ -90,10, -90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0, 0.7, 0, 1));
	Particle* p16 = new Particle(Vector3{ -90,10,90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0.5, 0, 0.7, 1));
	BungeeForceGenerator* bf81 = new BungeeForceGenerator(1, 10, p16);
	registry.addRegistry(p15, bf81);
	BungeeForceGenerator* bf82 = new BungeeForceGenerator(1, 10, p15);
	registry.addRegistry(p16, bf82);
	forceGenerators.push_back(bf81);
	forceGenerators.push_back(bf82);
	particles.push_back(p15);
	particles.push_back(p16);

}

void ParticleSystem::scene7()
{

	auto a = std::chrono::high_resolution_clock::now();
	float startTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();

	//1
	Particle* p1 = new Particle(Vector3{ 60,100,60 }, Vector3{ 0,0,0 }, 40, 1, 500, startTime, CreateShape(PxSphereGeometry(8.0)), Vector4(0, 0.6, 0.6, 1));
	AnchoredSpringForceGenerator* f1 = new AnchoredSpringForceGenerator(10, 10, { 60,100,60 });
	registry.addRegistry(p1, f1);
	registry.addRegistry(p1, gravity);
	forceGenerators.push_back(f1);
	particles.push_back(p1);

	//1
	Particle* p2 = new Particle(Vector3{ -60,100,60 }, Vector3{ 0,0,0 }, 40, 1, 500, startTime, CreateShape(PxSphereGeometry(8.0)), Vector4(0.6, 1, 0, 1));
	AnchoredSpringForceGenerator* f2 = new AnchoredSpringForceGenerator(10, 10, { -60,100,60 });
	registry.addRegistry(p2, f2);
	registry.addRegistry(p2, gravity);
	forceGenerators.push_back(f2);
	particles.push_back(p2);

	//1
	Particle* p3 = new Particle(Vector3{ 60,100,-60 }, Vector3{ 0,0,0 }, 40, 1, 500, startTime, CreateShape(PxSphereGeometry(8.0)), Vector4(0, 0.6, 0.6, 1));
	AnchoredSpringForceGenerator* f3 = new AnchoredSpringForceGenerator(10, 10, { 60,100,-60 });
	registry.addRegistry(p3, f3);
	registry.addRegistry(p3, gravity);
	forceGenerators.push_back(f3);
	particles.push_back(p3);

	//1
	Particle* p4 = new Particle(Vector3{ -60,100,-60 }, Vector3{ 0,0,0 }, 40, 1, 500, startTime, CreateShape(PxSphereGeometry(8.0)), Vector4(0.6, 1, 0, 1));
	AnchoredSpringForceGenerator* f4 = new AnchoredSpringForceGenerator(10, 10, { -60,100,-60 });
	registry.addRegistry(p4, f4);
	registry.addRegistry(p4, gravity);
	forceGenerators.push_back(f4);
	particles.push_back(p4);



	//3
	Particle* p5 = new Particle(Vector3{ 90,10, 90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0, 0.7, 0, 1));
	Particle* p6 = new Particle(Vector3{ -90,10,-90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0.5, 0, 0.7, 1));
	BungeeForceGenerator* bf31 = new BungeeForceGenerator(1, 10, p6);
	registry.addRegistry(p5, bf31);
	BungeeForceGenerator* bf32 = new BungeeForceGenerator(1, 10, p5);
	registry.addRegistry(p6, bf32);
	forceGenerators.push_back(bf31);
	forceGenerators.push_back(bf32);
	particles.push_back(p5);
	particles.push_back(p6);

	//4																
	Particle* p7 = new Particle(Vector3{ 90,10, -90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0.5, 0, 0.7, 1));
	Particle* p8 = new Particle(Vector3{ -90,10,90 }, Vector3{ 0,0,0 }, 2, 1, 500, startTime, CreateShape(PxSphereGeometry(4.0)), Vector4(0, 0.7, 0, 1));
	BungeeForceGenerator* bf41 = new BungeeForceGenerator(1, 10, p8);
	registry.addRegistry(p7, bf41);
	BungeeForceGenerator* bf42 = new BungeeForceGenerator(1, 10, p7);
	registry.addRegistry(p8, bf42);
	forceGenerators.push_back(bf41);
	forceGenerators.push_back(bf42);
	particles.push_back(p7);
	particles.push_back(p8);
}

void ParticleSystem::scene8()
{
	clearScene();
	scene8Intgr = true;

	RBParticle* rbPart = new RBParticle({ 60, 10, 0 }, { 0, 15, 0 }, { 0,0,0 }, 10, 5, gPhysics, gScene, CreateShape(PxBoxGeometry(1, 1, 1)), { 0.6, 0.2, 0, 1 });
	addRBGausianParticleGenerator(new RBGaussianParticleGenerator(rbPart, Vector3{ 5, 5, 5 }, Vector3{ 1, 1, 1 }, 50, gPhysics, gScene));
	//rbPart->setErrase();
	delete rbPart;

	RBParticle* rbPart2 = new RBParticle({ -60, 10, 0 }, { 0, 15, 0 }, { 0,0,0 }, 10, 5, gPhysics, gScene, CreateShape(PxBoxGeometry(1, 1, 1)), { 0.6, 0.2, 0, 1 });
	addRBGausianParticleGenerator(new RBGaussianParticleGenerator(rbPart2, Vector3{ 5, 5, 5 }, Vector3{ 1, 1, 1 }, 50, gPhysics, gScene));
	//rbPart->setErrase();
	delete rbPart2;

}
void ParticleSystem::scene9()
{

	RBParticle* rbPart = new RBParticle({ 0, 0, 0 }, { 0, 40, 0 }, { 0,0,0 }, 50, 5, gPhysics, gScene, CreateShape(PxCapsuleGeometry(1, 2)), { 0.2, 0.6, 0, 1 });
	addRBGausianParticleGenerator(new RBGaussianParticleGenerator(rbPart, Vector3{ 10, 10, 10 }, Vector3{ 1, 1, 1 }, 50, gPhysics, gScene));
	//rbPart->setErrase();
	delete rbPart;

}
void ParticleSystem::scene10()
{
	addWhirlwindForceGenerator(new WhirlwindForceGenerator(85, Vector3{ 0, 0, 0 }, 10, 20));

}