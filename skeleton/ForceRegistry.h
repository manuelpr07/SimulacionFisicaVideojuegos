#pragma once
#include <unordered_map>
#include <list>
#include "ForceGenerator.h"
#include "Particle.h"

class ForceRegistry {
public:
	std::unordered_multimap<Particle*, ForceGenerator*> registry;
	//std::unordered_multimap<Particle*, ForceGenerator*> RBregistry;
	void updateForces(float duration);
	void addRegistry(Particle* p, ForceGenerator* fg);
	//void addRBRegistry(RBParticle* p, ForceGenerator* fg);
	void addRegistry(Particle* p, std::vector<ForceGenerator*> fgs);
	void addRegistry(std::list<Particle*> ps, ForceGenerator* fg);
	void addRegistry(std::list<Particle*> ps, std::vector<ForceGenerator*> fgs);

	inline void deleteParticle(Particle* p);

	void deleteRegistry(const ForceGenerator* const fg);
	void clearRegistry();
};