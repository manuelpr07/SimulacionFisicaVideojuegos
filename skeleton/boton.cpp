#include "Boton.h"
#include <iostream>

Boton::Boton(Vector3 pos, Vector4 col): Particle(pos, Vector3{ 0,0,0 }, 0, 1, 1, 0, CreateShape(PxBoxGeometry(1.5f, 0.8f, 1.5f)), col){
    // Puedes personalizar la inicializaci�n del bot�n aqu� si es necesario
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
        // Verifica si la part�cula ha colisionado con el bot�n (geometr�a de cubo)
        PxBoxGeometry boxGeometry(PxVec3(1.0f, 1.0f, 1.0f)); // Ajusta las dimensiones seg�n tu geometr�a

        // Transforma la posici�n de la part�cula al espacio del bot�n
        PxTransform particleTransform = particula->getPos();

        // Calcula el centro del cubo manualmente
        PxVec3 boxCenter = pose.p;
        PxVec3 boxExtents = boxGeometry.halfExtents;

        // Calcula las coordenadas m�nimas y m�ximas del cubo en cada eje
        float minX = boxCenter.x - boxExtents.x;
        float maxX = boxCenter.x + boxExtents.x;
        float minY = boxCenter.y - boxExtents.y;
        float maxY = boxCenter.y + boxExtents.y;
        float minZ = boxCenter.z - boxExtents.z;
        float maxZ = boxCenter.z + boxExtents.z;

        // Verifica si la posici�n global de la part�cula est� dentro del cubo
        PxVec3 particlePositionGlobal = particleTransform.p;

        if (particlePositionGlobal.x >= minX && particlePositionGlobal.x <= maxX &&
            particlePositionGlobal.y >= minY && particlePositionGlobal.y <= maxY &&
            particlePositionGlobal.z >= minZ && particlePositionGlobal.z <= maxZ) {
            // La part�cula ha colisionado con el bot�n
            std::cout << "Colisi�n detectada" << std::endl;
            auto a = std::chrono::high_resolution_clock::now();
            activationTime = std::chrono::duration_cast<std::chrono::duration<double>>(a.time_since_epoch()).count();
            activado = true;
            return true;
        }
        else return false;
    }
    else return false;
}
