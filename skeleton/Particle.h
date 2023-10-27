#pragma once

#include <chrono>
#include "RenderUtils.hpp"

using namespace physx;

class Particle
{
public:
	Particle(Vector3 pos, Vector3 realVel, int realMass, float damp, double lifeT);
	Particle(Vector3 pos, Vector3 realVel, int realMass, float damp, double lifeT, double startT);
	Particle(Vector3 pos, Vector3 realVel, Vector4 color, float damp, double lifeT);
	~Particle();
	Vector3 getVel() { return SimulatedVel; };
	float getDamping() { return damping; };
	Vector3 getPos() { return pose.p; };
	void setPos(Vector3 p) { pose.p = p; };
	int getMass() { return SimulatedMass; };
	double getLifeTime() { return lifeTime; };
	void setLifeTime(double lf) { lifeTime = lf; };
	double getStartTime() { return startTime; };
	void integrate(float t);
	bool getErrase() { return errase; };

private:
	bool errase = false;
	Vector3 SimulatedVel;
	Vector3 acc;
	float SimulatedMass;
	PxTransform pose;
	float damping;
	double lifeTime;
	double startTime;
	RenderItem* renderItem;
	const Vector3 gravity = { 0, -10, 0 };
};

