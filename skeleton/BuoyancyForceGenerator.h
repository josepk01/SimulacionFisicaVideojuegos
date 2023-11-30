#pragma once
#include "ForceGenerator.h"
#include "Particle.h"

class BuoyancyForceGenerator : public ForceGenerator {
private:
    float maxDepth; // La profundidad m�xima a la que el objeto est� completamente sumergido.
    float volume; // El volumen total del objeto.
    float waterHeight; // La altura del agua.
    float liquidDensity; // La densidad del l�quido.

public:
    BuoyancyForceGenerator(const std::string& name, float maxDepth, float volume, float waterHeight, float liquidDensity)
        : ForceGenerator(name), maxDepth(maxDepth), volume(volume), waterHeight(waterHeight), liquidDensity(liquidDensity) {}

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
};
