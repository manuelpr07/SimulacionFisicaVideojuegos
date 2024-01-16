#include "ForceGenerator.h"

void GravityForceGenerator::updateForce(Particle* p, float t) {
	p->addForce(gravity * p->getMass());

}

void WindForceGenerator::updateForce(Particle* p, float t) {

	Vector3 vel = windSpeed - p->getVel();
	Vector3 force = k * vel;

	p->addForce(force);
}

void WhirlwindForceGenerator::updateForce(Particle* p, float t) {
	if ((p->getPos().p.x >= center.x - size && p->getPos().p.x <= center.x + size) || (p->getPos().p.y >= center.y - size && p->getPos().p.y <= center.y + size) || (p->getPos().p.z >= center.z - size && p->getPos().p.z <= center.z + size))
	{
		Vector3 pos = p->getPos().p;
		Vector3 vel = p->getVel();
		windSpeed = k * Vector3(-(pos.z - center.z), -(pos.y - center.y), pos.x - center.x);
		Vector3 force = k1 * (windSpeed - vel);
		p->addForce(force);
	}
}

void ExplosionForceGenerator::updateForce(Particle* p, float t) {

    Vector3 partDist = p->getPos().p - center;
    float distance = partDist.magnitude();

    // Si la part�cula est� dentro del radio de la explosi�n
    if (distance < radius && startTime > 0) {
        if (distance < 0.001f) distance = 0.001f;

		auto a = std::chrono::high_resolution_clock::now();
		double actualTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();

		float expTime = actualTime - startTime; // Utilizar el tiempo de simulación
		// Asegúrate de que el tiempo desde la explosión sea positivo
		if (expTime < 0) return;
		float magnitude = (K / (distance * distance)) * exp(-expTime / tau);
		Vector3 force = partDist * magnitude; // Normalizar partDist para la dirección
		p->addForce(force);

		// Si la explosión ha terminado, reiniciar startTime
		if (expTime >= 4 * tau) {
			startTime = 0;
		}
    }
}
SpringForceGenerator::SpringForceGenerator(double k, double r, Particle* p)
{
	K = k;
	resLength = r;
	other = p;
}
void SpringForceGenerator::updateForce(Particle* p, float t) {
	Vector3 relPos = other->getPos().p - p->getPos().p;
	Vector3 f;

	const float length = relPos.normalize();
	const float delta_x = length - resLength;

	f = relPos * delta_x * K;
	p->addForce(f);


}
BungeeForceGenerator::BungeeForceGenerator(double k, double r, Particle* p)
{
	K = k;
	resLength = r;
	other = p;
}
void BungeeForceGenerator::updateForce(Particle* p, float t) {
	Vector3 f = p->getPos().p - other->getPos().p;
	float length = f.normalize();
	length = length - resLength;
	if (length <= 0.0f)
	{
		f = { 0,0,0 };
		return;
	}
	f *= -(length * K);
	p->addForce(f);

}
AnchoredSpringForceGenerator::AnchoredSpringForceGenerator(double k, double resting, const Vector3& anchor_pos) : SpringForceGenerator(k, resting, nullptr)
{
	other = new Particle(anchor_pos, Vector3{ 0,0,0 }, 50, 1, 1, 0, CreateShape(PxBoxGeometry(1.0, 1.0, 1.0)), Vector4(0.9, 0.9, 0.9, 0));
	other->setSize(1);
}

AnchoredSpringForceGenerator::~AnchoredSpringForceGenerator()
{
	delete other;
}	
void AnchoredSpringForceGenerator::clear()
{
	delete other;
}

BuoyancyForceGenerator::BuoyancyForceGenerator(float d, Vector3 pos)
{
	liquidDensity = d;
	liquidParticle = new Particle(Vector3{ 0,50,0 }, Vector3{ 0,0,0 }, 50, 1, 1, 0, CreateShape(PxBoxGeometry(10.0f, 0.5f, 10.0f)), Vector4(0.2, 0.3, 0.5, 1.0));
	liquidParticle->setSize(10);
}

void BuoyancyForceGenerator::updateForce(Particle* particle, float t) {
	float h = particle->getPos().p.y;
	float h0 = liquidParticle->getPos().p.y;
	float height = particle->getSize();
	float volume = (4.0 / 3) * (std::atan(1) * 4) * particle->getSize() / 2 * particle->getSize() / 2 * particle->getSize() / 2;
	Vector3 f(0, 0, 0);
	float immersed = 0.0;
	if (h - h0 > height * 0.5)
	{
		immersed = 0.0;
	}
	else if (h0 - h > height * 0.5) {
		immersed = 1.0;
	}
	else{
		immersed = (h0 - h) / height + 0.5;
	}
	f.y = liquidDensity * volume * immersed * gravity;

	particle->addForce(f);
}

void BuoyancyForceGenerator::clear()
{
	delete liquidParticle;
}
