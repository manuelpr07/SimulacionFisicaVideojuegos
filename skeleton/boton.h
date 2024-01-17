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

    // Sobrescribe el m�todo integrate para agregar comportamiento espec�fico del bot�n
    void integrate(float t) override;
    bool DetectarColision(Particle* particula);
};