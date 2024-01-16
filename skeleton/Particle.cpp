#include "Particle.h"
#include <iostream>

Particle::Particle(Vector3 pos, Vector3 realVel, float realMass, float damp, double lifeT, Vector4 col) {

	pose = PxTransform(pos);
	mass = realMass;
	damping = damp;
	vel = realVel;
	lifeTime = lifeT;
	color = col;
	size = 1;
	force = { 0,0,0 };
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0)), &pose, col);
}

Particle::Particle(Vector3 pos, Vector3 realVel, float realMass, float damp, double lifeT, double startT, PxShape* shape, Vector4 col) {

	pose = PxTransform(pos);
	mass = realMass;
	damping = damp;
	vel = realVel;
	lifeTime = lifeT;
	color = col;
	force = { 0,0,0 };
	renderItem = new RenderItem(shape, &pose, col);
	startTime = startT;
}

Particle::Particle(Vector3 pos, Vector3 realVel, float realMass, float damp, double lifeT, double startT, Vector4 col) {

	pose = PxTransform(pos);
	mass = realMass;
	damping = damp;
	vel = realVel;
	lifeTime = lifeT;
	color = col;
	size = 1;
	force = { 0,0,0 };
	renderItem = new RenderItem(CreateShape(PxSphereGeometry(1.0)), &pose, col);
	startTime = startT;
}

Particle::Particle(Vector3 pos, Vector3 realVel, Vector4 color, float damp, double lifeT)
{
	pose = PxTransform(pos);
	damping = damp;
	vel = realVel;
	lifeTime = lifeT;
	mass = 1;
	size = 1;
	force = { 0,0,0 };
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

	if (!estatico)
	{
		// Get the accel considering the force accum
		Vector3 resulting_accel = force;
		resulting_accel *= (1 / mass);
		//std::cout << resulting_accel.x << " " << resulting_accel.y << " " << resulting_accel.z << endl;
		//euler
		//pose.p = pose.p+vel*t;
		//vel = vel+ resulting_accel*t;
		//euler semi- implicito
		vel += resulting_accel * t;
		vel *= pow(damping, t);
		pose.p += vel * t;
		clearForce();
		auto a = std::chrono::high_resolution_clock::now();
		double actualTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();

		if (actualTime > startTime + getLifeTime()) {
			errase = true;
		}
	}


}
