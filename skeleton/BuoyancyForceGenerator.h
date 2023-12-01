#pragma once
#include "ForceGenerator.h"
#include "Particle.h"
#include <PxPhysicsAPI.h>

class BuoyancyForceGenerator : public ForceGenerator {
private:
    float maxDepth; // La profundidad m�xima a la que el objeto est� completamente sumergido
    float volume; // El volumen total del objeto
    float waterHeight; // La altura del agua
    float liquidDensity; // La densidad del l�quido
    RenderItem* waterSurface; // Representaci�n visual de la superficie del agua

public:
    BuoyancyForceGenerator(const std::string& name, float maxDepth, float volume, float waterHeight, float liquidDensity)
        : ForceGenerator(name), maxDepth(maxDepth), volume(volume), waterHeight(waterHeight), liquidDensity(liquidDensity) {
        // Crear la representaci�n visual de la superficie del agua
        physx::PxTransform pose(physx::PxVec3(0.0f, waterHeight, 0.0f)); // Posici�n central en el origen con la altura de la superficie del agua
        waterSurface = new RenderItem(CreateShape(physx::PxBoxGeometry(50.0f, 0.1f, 50.0f)), &pose, Vector4(0, 1, 0, 1)); // Crea un cuadrado azul para el agua
    }

    virtual void updateForce(Particle* particle, float duration) {
        // Calcula la profundidad de la part�cula (es decir, la distancia desde la parte superior del objeto hasta la superficie del agua)
        float depth = particle->getPosition().y;

        // Comprueba si est� fuera del agua
        if (depth >= waterHeight + maxDepth) return;

        Vector3 force(0, 0, 0);

        // Comprueba si est� completamente sumergido
        if (depth <= waterHeight - maxDepth) {
            force.y = liquidDensity * volume;
        }
        else {
            // Solo est� parcialmente sumergido
            force.y = liquidDensity * volume * (depth - waterHeight + maxDepth) / (2 * maxDepth);
        }

        // Aplica la fuerza de flotaci�n (empuje hacia arriba)
        particle->addForce(force * 9.8f);
    }

    // Aseg�rate de limpiar la memoria en el destructor
    ~BuoyancyForceGenerator() {
        delete waterSurface;
    }
};
