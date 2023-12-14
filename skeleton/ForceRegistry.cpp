#include "ForceRegistry.h"

void ForceRegistry::updateForces(float duration)
{
	for (auto it = registry.begin(); it != registry.end(); it++) {
		it->second->updateForce(it->first, duration);
	}
}

void ForceRegistry::addRegistry(Particle* p, ForceGenerator* fg)
{
	registry.insert({ p, fg });
}

void ForceRegistry::addRegistry(Particle* p, std::vector<ForceGenerator*> fgVector)
{
	for (int i = 0; i < fgVector.size(); ++i) {
		registry.insert({ p, fgVector[i] });
	}
}

void ForceRegistry::addRegistry(std::list<Particle*> pList, ForceGenerator* fg)
{
	int aux = pList.size();
	for (int i = 0; i < aux; ++i) {
		registry.insert({ pList.front(), fg });
		pList.pop_front();
	}
}

void ForceRegistry::addRegistry(std::list<Particle*> pList, std::vector<ForceGenerator*> fgVector)
{
	int aux = pList.size();
	for (int i = 0; i < aux; ++i) {
		for (int j = 0; j < fgVector.size(); ++j) {
			registry.insert({ pList.front(), fgVector[j] });
		}
		pList.pop_front();
	}
}
inline void ForceRegistry::deleteParticle(Particle* p)
{
	registry.erase(p);
}
void ForceRegistry::deleteRegistry(const ForceGenerator* const fg)
{
	for (auto it = registry.begin(); it != registry.end();) {
		if (it->second == fg) it = registry.erase(it);
		else ++it;
	}
}