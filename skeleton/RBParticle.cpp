#include "RBParticle.h"

RBParticle::RBParticle(Vector3 pos, Vector3 linearVel, Vector3 angularVel, float m, float lifeT, PxPhysics* physics, PxScene* scene, PxShape* shape, Vector4 col = { 1, 0, 0, 1 }) : Particle(pos, linearVel, m, 0.998f, lifeT, col)
{
	//generar un rigidbody dinamico
	rd = physics->createRigidDynamic(PxTransform(pos));
	rd->setLinearVelocity(linearVel);
	rd->setAngularVelocity(angularVel);
	rd->attachShape(*shape);

	gScene = scene;
	//distribucion de masas
	PxRigidBodyExt::updateMassAndInertia(*rd, m);
	gScene->addActor(*rd);

	renderItem->release();
	renderItem = new RenderItem(shape, rd, col);

	auto a = std::chrono::high_resolution_clock::now();
	startTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();
}



RBParticle::~RBParticle()
{
	gScene->removeActor(*rd);
}

void RBParticle::addForce(const Vector3& f)
{
	rd->addForce(f);
}

void RBParticle::integrate(float t)
{

	auto a = std::chrono::high_resolution_clock::now();
	double actualTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();

	if (actualTime > startTime + getLifeTime()) {
		errase = true;
	}


}