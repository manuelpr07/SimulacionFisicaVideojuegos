#pragma once
#include "Particle.h"
class RBParticle : public Particle
{
private:
	PxRigidDynamic* rd = nullptr;
	PxScene* gScene = nullptr;
	float startTime;
	PxShape* shape;
public:
	RBParticle(Vector3 pos, Vector3 linearVel, Vector3 angularVel, float m, float lifeT, PxPhysics* physics, PxScene* scene, PxShape* s, Vector4 col);
	~RBParticle();
	void addForce(const Vector3& f) override;
	void integrate(float t) override;
	PxShape* getShape() { return shape; };

	//inline void setGenerator(GaussianRigidGenerator* creator) { rg = creator; }


};

