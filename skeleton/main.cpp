#include <ctype.h>

#include <PxPhysicsAPI.h>
#include <iostream>
#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "UniformParticleGenerator.h"
#include "gaussianParticleGenerator.h"
#include "RBGaussianParticleGenerator.h"
//#include "FireWork.h"
#include "ParticleSystem.h"


std::string display_text = "This is a test";


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

//std::vector<Particle*> particulas;
Particle* part;
ParticleSystem* partSys;
RBGaussianParticleGenerator* GeneradorRigido;
float springPower = 20.0f;

FireworksRules fr;
// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(),true,gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	
	//sistema de particulas
	partSys = new ParticleSystem();

	////fuente
	//part = new Particle(Vector3{0,0,0 }, Vector3{ 0,30,0 }, 12, 1, 5, Vector4(0.5, 0, 0.7, 1));
	//partSys->addParticleGenerator(new gaussianParticleGenerator(Vector3{ 2, 0, 2 }, Vector3{ 5, 5, 5 }, part, "fuente"));
	//delete part;
	
	//chorro1
	//part = new Particle(Vector3{ 30,0,10 }, Vector3{ 20,10,0 }, 4, 1, 10, Vector4(1, 0, 0, 1));
	//partSys->addParticleGenerator(new gaussianParticleGenerator(Vector3{ 1, 1, 1 }, Vector3{ 1, 1, 1 }, part, "chorro1"));
	//delete part;

	////chorro2
	//part = new Particle(Vector3{ 30,0,20 }, Vector3{ 20,10,0 }, 40, 1, 10, Vector4(1, 0.5, 0, 1));
	//partSys->addParticleGenerator(new gaussianParticleGenerator(Vector3{ 1, 1, 1 }, Vector3{ 1, 1, 1 }, part, "chorro2"));
	//delete part;

	////chorro3
	//part = new Particle(Vector3{ 30,0,30 }, Vector3{ 20,10,0 }, 400, 1, 10, Vector4(1, 1, 0, 1));
	//partSys->addParticleGenerator(new gaussianParticleGenerator(Vector3{ 1, 1, 1 }, Vector3{ 1, 1, 1 }, part, "chorro3"));
	//delete part;

	//lluvia
	//part = new Particle(Vector3{0,0,0 }, Vector3{ 0,0,0 }, 12, 1, 50, Vector4(0, 0, 0.8, 1));
	//partSys->addParticleGenerator(new gaussianParticleGenerator(Vector3{ 25, 0, 25 }, Vector3{ 0, 0, 0 }, part, "lluvia"));
	//delete part;

	//lluvia pesada
	//part = new Particle(Vector3{ 0,50,0 }, Vector3{ 0,0,0 }, 100, 1, 10, Vector4(0.8, 0, 0, 1));
	//partSys->addParticleGenerator(new gaussianParticleGenerator(Vector3{ 25, 0, 25 }, Vector3{ 0, 0, 0 }, part, "lluvia2"));
	//delete part;

	//gravedad
	//partSys->addGravityForceGenerator(new GravityForceGenerator(Vector3(0, -9.81f, 0)));

	//viento
	partSys->addWindForceGenerator(new WindForceGenerator(Vector3{ 10, 0, 10 }, 10));

	//torbellino
	partSys->addWhirlwindForceGenerator(new WhirlwindForceGenerator(10, Vector3{ 0, 0, 0 }, 5, 10));

	//explosion
	//ExplosionForceGenerator* exp = new ExplosionForceGenerator(Vector3(0, 0, 0), 50000, 1.0f, 0);
	//partSys->addForceGenerator(exp);
	//partSys->addExplosionGenerator(exp);

	//anchoredSpring
	//SpringForceGenerator* springGen = new SpringForceGenerator("Spring", anchorPoint, springConstant, restLength);
	//particleSystem->addForceGenerator(springGen);
	//spring = springGen;

	//firework
	fr.colour = { 1, 0, 0, 1 };
	fr.numParticles = 10;
	fr.vel = { 0,40,0 };
	fr.damping = 1;
	fr.lifeTime = 4;
	fr.level = 0;
	//Fireworks.push_back(new Firework(Vector3{ 0,0,0 }, fr, Fireworks));

	//generar el suelo estatico
	PxRigidStatic* suelo = gPhysics->createRigidStatic(PxTransform({ 0,0,0 }));
	PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
	suelo->attachShape(*shape);
	gScene->addActor(*suelo);

	//renderizar el suelo
	RenderItem* item;
	item = new RenderItem(shape, suelo, { 0.8, 0.8, 0.8, 1 });


	//addRBParticleGenerator
	//generador rigido dinamico
	PxShape* shape_ad = CreateShape(PxBoxGeometry(5, 0, 5));
	RBParticle* rbPart = new RBParticle({ 0, 50, 0 }, { 0, -15, 0 }, {0,0,0}, 1, 5, gPhysics, gScene, shape_ad, { 1, 0, 0, 1 });
	partSys->addRBParticleGenerator(new RBGaussianParticleGenerator(rbPart, Vector3{ 5, 5, 5 }, Vector3{ 5, 5, 5 },50, gPhysics, gScene));
	delete rbPart;
	//part = new Particle(Vector3{ 30,0,30 }, Vector3{ 20,10,0 }, 400, 1, 10, Vector4(1, 1, 0, 1));
	//partSys->addParticleGenerator(new gaussianParticleGenerator(Vector3{ 1, 1, 1 }, Vector3{ 1, 1, 1 }, part, "chorro3"));
	//delete part;
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	partSys->integrate(t);
	PX_UNUSED(interactive);
	gScene->simulate(t);
	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{

	delete partSys;

	PX_UNUSED(interactive);
	gScene->release();
	gDispatcher->release();
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();
	}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	switch(toupper(key))
	{
	case 'B': {
		//PxVec3 direccionDeMira = camera.q.rotate(PxVec3(0, 0, 1));
		//direccionDeMira.normalize();
		//direccionDeMira = direccionDeMira * -fuerzaBala;
		partSys->generate(fr);

	}; break;
	case 'V': {
		auto a = std::chrono::high_resolution_clock::now();
		double actualTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();
		partSys->explosion(actualTime);
	}; break; 
	case 'C': {
		partSys->generateSpringDemo();
	}; break;
	case 'X': {
		partSys->generateAnchoredSpringDemo();
	}; break;
	case 'Z': {
		partSys->generateBuoyancyDemo();
	}; break;
	case 'U': {// Aumentar la constante del muelle
		partSys->changeSpringPower(+500);
	};	break;
	case 'Y': {// Disminuir la constante del muelle
		partSys->changeSpringPower(-50);
	};	break;
	case 'I': {// Disminuir la constante del muelle
		partSys->anchoredSForce(100);
	};
	case ' ':
	{
		break;
	}
	//default:
	//	break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}