#pragma once

#include "Particle.h"

class Boton : public Particle {
private:
    bool activado;
    double activationTime;
    const double buffer = 2;
public:
    Boton(Vector3 pos, Vector4 col);
    bool EstaActivado() const;

    // Sobrescribe el método integrate para agregar comportamiento específico del botón
    void integrate(float t) override;
    bool DetectarColision(Particle* particula);
};