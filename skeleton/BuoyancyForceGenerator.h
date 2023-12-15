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
        //waterSurface = new RenderItem(CreateShape(physx::PxBoxGeometry(20.0f, 20.0f, 20.0f)), &pose, Vector4(0, 1, 0, 1)); // Crea un cuadrado azul para el agua
    }

    virtual void updateForce(Particle* particle, PxRigidDynamic* actor, float duration) {
        // Obtiene la posici�n de la part�cula
        Vector3 position = particle->getPosition();

        // Comprueba si la part�cula est� dentro del rango horizontal de la superficie del agua
        // Por ejemplo, si la superficie del agua es un cuadrado centrado en el origen con lado 100
        bool isWithinHorizontalBounds = (position.x >= -20.0f && position.x <= 20.0f) &&
            (position.z >= -20.0f && position.z <= 20.0f);

        // Calcula la profundidad de la part�cula
        float depth = position.y;

        // Comprueba si est� fuera del agua o fuera de los l�mites horizontales
        if (depth >= waterHeight + maxDepth || !isWithinHorizontalBounds) return;

        Vector3 force(0, 0, 0);

        // Comprueba si est� completamente sumergido
        if (depth <= waterHeight - maxDepth) {
            force.y = liquidDensity * volume;
        }
        else {
            // Solo est� parcialmente sumergido
            force.y = liquidDensity * volume * (depth - waterHeight + maxDepth) / (2 * maxDepth);
        }

        // Aplica la fuerza de flotaci�n
        particle->addForce(force * 9.8f);
    }


    // Aseg�rate de limpiar la memoria en el destructor
    ~BuoyancyForceGenerator() {
        delete waterSurface;
    }
};
