#pragma once
#include "Particle.h"
#include <iostream>

class ForceGenerator{

public:

	virtual void updateForce(Particle* p, float t) = 0;

};

class GravityForceGenerator : public ForceGenerator {
public:
	GravityForceGenerator(const Vector3& g) : gravity(g) {}
	void updateForce(Particle* p, float t) override;
	inline void setGravity(Vector3 g) { gravity = g; }
protected:
	Vector3 gravity;
};

class WindForceGenerator : public ForceGenerator {
private:
	Vector3 windSpeed;
	float k;

public:
	WindForceGenerator(const Vector3& windVelocity, float k): windSpeed(windVelocity), k(k) {}
	void updateForce(Particle* particle, float duration) override;
	
};

class WhirlwindForceGenerator : public ForceGenerator {
private:
	Vector3 windSpeed;
	Vector3 center;
	float k;
	float k1;
	double size;
public:
	WhirlwindForceGenerator(double size, Vector3 center, float K, float k1) : center(center), size(size), k(K), k1(k1) {}
	void updateForce(Particle* p, float t) override;
};

class ExplosionForceGenerator : public ForceGenerator {
private:
	Vector3 center;
	float radius = 50;
	float K;
	float tau;
	float startTime;
	Vector3 force;
	static Vector3 expSpeed;
public:
	ExplosionForceGenerator(Vector3 explosionCenter, float K, float tau, float time) : center(explosionCenter), K(K), tau(tau), startTime(time) {}
	void updateForce(Particle* p, float t) override;
	void start(float time) {
		startTime = time;
	}
};
class SpringForceGenerator : public ForceGenerator {
public:
	SpringForceGenerator(double k, double r, Particle* p);
	void updateForce(Particle* p, float t) override;
	inline void setK(double k) { K += k; };
	virtual ~SpringForceGenerator() {};
protected:
	double K;
	double resLength;
	Particle* other;
};
class BungeeForceGenerator : public ForceGenerator {
public:
	BungeeForceGenerator(double k, double r, Particle* p);
	void updateForce(Particle* p, float t) override;
	inline void setK(double k) { K += k; };
	virtual ~BungeeForceGenerator() {};
protected:
	double K;
	double resLength;
	Particle* other;
};
class AnchoredSpringForceGenerator : public SpringForceGenerator {
public:
	AnchoredSpringForceGenerator(double k, double resting, const Vector3& anchor_pos);
	~AnchoredSpringForceGenerator();
	void clear();
};
class BuoyancyForceGenerator : public ForceGenerator
{
public:
	BuoyancyForceGenerator(float d, Vector3 pos);
	void updateForce(Particle* particle, float t);
	~BuoyancyForceGenerator() {};
	void clear();
protected:
	float liquidDensity;
	float gravity = 9.8;
	Particle* liquidParticle;
};