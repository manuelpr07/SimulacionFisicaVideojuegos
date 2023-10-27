#include "Particle.h"

Particle::Particle(Vector3 pos, Vector3 realVel, int realMass, float damp, double lifeT) {

	pose = PxTransform(pos);
	SimulatedMass = realMass;
	damping = damp;
	SimulatedVel = realVel;
	lifeTime = lifeT;
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0)), &pose, Vector4(0.5, 0, 0.7, 1));
}

Particle::Particle(Vector3 pos, Vector3 realVel, int realMass, float damp, double lifeT, double startT) {

	pose = PxTransform(pos);
	SimulatedMass = realMass;
	damping = damp;
	SimulatedVel = realVel;
	lifeTime = lifeT;
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0)), &pose, Vector4(0.5, 0, 0.7, 1));
	startTime = startT;
}

Particle::Particle(Vector3 pos, Vector3 realVel, Vector4 color, float damp, double lifeT)
{
	pose = PxTransform(pos);
	damping = damp;
	SimulatedVel = realVel;
	lifeTime = lifeT;
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0)), &pose, color);
	auto a = std::chrono::high_resolution_clock::now();
	startTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();
}

Particle::~Particle()
{
	renderItem->release();
	
}



void Particle::integrate(float t)
{
	// Trivial case, infinite mass --> do nothing
	//if (inverse_mass <= 0.0f) return;
	
	// Update position
	Vector3 increase = SimulatedVel * t;
	pose.p = pose.p + increase;

	// Update linear velocity
	SimulatedVel = SimulatedVel + gravity * t;

	// Impose drag (damping)
	SimulatedVel = SimulatedVel * powf(damping, t);

	auto a = std::chrono::high_resolution_clock::now();
	double actualTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();

	if (actualTime > startTime + getLifeTime()) {
		errase = true;
	}


}

//void Particle::integrate(float t)
//{
//	// Trivial case, infinite mass --> do nothing
//	//if (inverse_mass <= 0.0f) return;
//
//	// Update position
//	pose.p = pose.p + (vel * t);
//
//	// Update linear velocity
//	vel = vel + acc * t;
//
//	// Impose drag (damping)
//	vel = vel * powf(damping, t);
//
//}
