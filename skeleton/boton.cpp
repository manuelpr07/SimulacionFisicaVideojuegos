#include "Boton.h"
#include <iostream>

Boton::Boton(Vector3 pos, Vector4 col): Particle(pos, Vector3{ 0,0,0 }, 0, 1, 1, 0, CreateShape(PxBoxGeometry(1.5f, 0.8f, 1.5f)), col){
    // Puedes personalizar la inicialización del botón aquí si es necesario
    activado = false;
}


bool Boton::EstaActivado() const {
    return activado;
}

void Boton::integrate(float t) {
    if (activado)
    {

        auto a = std::chrono::high_resolution_clock::now();
        double actualTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();

        if (actualTime > activationTime + buffer) {
            activado = false;
        }
    }
}

bool Boton::DetectarColision(Particle* particula) {
    if (!activado)
    {
        // Verifica si la partícula ha colisionado con el botón (geometría de cubo)
        PxBoxGeometry boxGeometry(PxVec3(1.0f, 1.0f, 1.0f)); // Ajusta las dimensiones según tu geometría

        // Transforma la posición de la partícula al espacio del botón
        PxTransform particleTransform = particula->getPos();

        // Calcula el centro del cubo manualmente
        PxVec3 boxCenter = pose.p;
        PxVec3 boxExtents = boxGeometry.halfExtents;

        // Calcula las coordenadas mínimas y máximas del cubo en cada eje
        float minX = boxCenter.x - boxExtents.x;
        float maxX = boxCenter.x + boxExtents.x;
        float minY = boxCenter.y - boxExtents.y;
        float maxY = boxCenter.y + boxExtents.y;
        float minZ = boxCenter.z - boxExtents.z;
        float maxZ = boxCenter.z + boxExtents.z;

        // Verifica si la posición global de la partícula está dentro del cubo
        PxVec3 particlePositionGlobal = particleTransform.p;

        if (particlePositionGlobal.x >= minX && particlePositionGlobal.x <= maxX &&
            particlePositionGlobal.y >= minY && particlePositionGlobal.y <= maxY &&
            particlePositionGlobal.z >= minZ && particlePositionGlobal.z <= maxZ) {
            // La partícula ha colisionado con el botón
            std::cout << "Colisión detectada" << std::endl;
            auto a = std::chrono::high_resolution_clock::now();
            activationTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();
            activado = true;
            return true;
        }
        else return false;
    }
    else return false;
}
