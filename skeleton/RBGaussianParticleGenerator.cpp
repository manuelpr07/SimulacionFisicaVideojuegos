#include "RBGaussianParticleGenerator.h"

RBGaussianParticleGenerator::RBGaussianParticleGenerator(RBParticle* mod, Vector3 posW,Vector3 velW, int max, PxPhysics* phys, PxScene* scene)
{
	desvPos = posW;
	linearVel = linearVel;

	maxParticles = max;
	currentParticles = 0;
	physics = phys;
	gScene = scene;
	modelParticle = new RBParticle(mod->getPos().p, mod->getVel(), {0,0,0 }, mod->getMass(), mod->getLifeTime(), physics, gScene, CreateShape(PxBoxGeometry(1, 1, 1)), mod->getColor());
	currentParticles++;
}

RBGaussianParticleGenerator::~RBGaussianParticleGenerator()
{
	auto it = listParticles.begin();
	while (it != listParticles.end()) {
		it = listParticles.erase(it);
	}
}

RBParticle* RBGaussianParticleGenerator::generateParticles(std::list<RBParticle*>& particles)
{
		//generar un rigidbody dinamico
		PxRigidDynamic* new_solid;

		Vector3 newPos;
		Vector3 newVel;

		std::mt19937 mt(std::random_device{}());

		//calculo de la nueva posicion
		std::normal_distribution<double> distributionPX(modelParticle->getPos().p.x, desvPos.x);
		newPos.x = distributionPX(mt);

		std::normal_distribution<double> distributionPY(modelParticle->getPos().p.y, desvPos.y);
		newPos.y = distributionPY(mt);

		std::normal_distribution<double> distributionPZ(modelParticle->getPos().p.z, desvPos.z);
		newPos.z = distributionPZ(mt);

		//calculo de la nueva velocidad
		std::normal_distribution<double> distributionVX(modelParticle->getVel().x, desvVel.x);
		newVel.x = distributionVX(mt);

		std::normal_distribution<double> distributionVY(modelParticle->getVel().y, desvVel.y);
		newVel.y = distributionVY(mt);

		std::normal_distribution<double> distributionVZ(modelParticle->getVel().z, desvVel.z);
		newVel.z = distributionVZ(mt);

		PxShape* shape = CreateShape(PxBoxGeometry(1, 1, 1));
		auto a = std::chrono::high_resolution_clock::now();
		double actualTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();

		RBParticle* part = new RBParticle(newPos, newVel, { 0, 0, 0 }, modelParticle->getMass(), actualTime, physics, gScene, shape, modelParticle->getColor());
		//new_solid = physics->createRigidDynamic(PxTransform(newPos));
		//new_solid->setLinearVelocity(newVel);
		//new_solid->setAngularVelocity({ 0, 0, 0 });
		//PxShape* shape_ad = CreateShape(PxBoxGeometry(1, 1, 1));
		//new_solid->attachShape(*shape_ad);

		////distribucion de masas
		//PxRigidBodyExt::updateMassAndInertia(*new_solid, 0.15);
		//gScene->addActor(*new_solid);

		////renderizado
		//RenderItem* dynamic_item;
		//dynamic_item = new RenderItem(shape_ad, new_solid, modeParticle->getColor());
		currentParticles++;
		particles.push_back(part);
		return part;

}
