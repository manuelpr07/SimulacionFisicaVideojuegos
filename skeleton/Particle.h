#pragma once

#include <chrono>
#include "RenderUtils.hpp"
#include "core.hpp"

#include <vector>
using namespace physx;
using namespace std;

class Particle
{
public:
	Particle(Vector3 pos, Vector3 realVel, int realMass, float damp, double lifeT, Vector4 col);
	Particle(Vector3 pos, Vector3 realVel, int realMass, float damp, double lifeT, double startT, PxShape* shape, Vector4 col);
	Particle(Vector3 pos, Vector3 realVel, int realMass, float damp, double lifeT, double startT, Vector4 col);
	Particle(Vector3 pos, Vector3 realVel, Vector4 color, float damp, double lifeT);
	~Particle();

	void setVel(Vector3 v) { vel = v; }
	Vector3 getVel() { return vel; };

	void setAcc(Vector3 a) { acc = a; }
	Vector3 getAcc() { return acc; };

	void setDamping(float d) { damping = d; };
	float getDamping() { return damping; };

	void setPos(Vector3 p) { pose.p = p; };
	PxTransform getPos() { return pose; };

	void setMass(float m) { mass = m; invMass = 1 / m;};
	int getMass() { return mass; };
	float getInvMass() { return invMass; };

	void setLifeTime(double lf) { lifeTime = lf; };
	double getLifeTime() { return lifeTime; };

	double getStartTime() { return startTime; };

	void integrate(float t);

	void setErrase() { errase = true; };
	bool getErrase() { return errase; };

	float getSize() { return size; };
	void setSize(float s) { size = s; };
	Vector4 getColor() { return color; ; }

	void clearForce() { force *= 0.0f; };
	void addForce(const Vector3& f) {
		force += f;
	};

	bool getStatic() { return estatico; };
	void setStatic(bool s) { estatico = s; };
	
private:
	Vector3 vel;
	Vector3 acc;
	Vector3 force;
	float damping;
	float mass;
	float invMass;
	double lifeTime;
	double startTime;
	PxTransform pose;
	RenderItem* renderItem;
	bool estatico = false;
	//const Vector3 gravity = { 0, -10, 0 };

	int size;
	Vector4 color;

	bool errase = false;

	//particleType type;
	//particleShape shape;

};

