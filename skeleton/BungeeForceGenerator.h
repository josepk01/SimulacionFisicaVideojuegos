#pragma once
#include "ForceGenerator.h"
#include "Particle.h"

class BungeeForceGenerator : public ForceGenerator {
private:
    Particle* other; // La otra partícula a la que está conectada
    float k;         // Constante de muelle
    float restLength;// Longitud de reposo

public:
    BungeeForceGenerator(const std::string& name, Particle* other, float k, float restLength)
        : ForceGenerator(name), other(other), k(k), restLength(restLength) {}

    void updateForce(Particle* particle, PxRigidDynamic* actor, float duration) override {
        // Calcula el vector de la partícula a la otra partícula
        Vector3 force = particle->getPosition() - other->getPosition();

        // Calcula la magnitud de la fuerza
        float magnitude = force.magnitude();
        if (magnitude <= restLength) return; // No hay fuerza si la distancia es menor que la longitud de reposo

        // Calcula la magnitud de la fuerza
        magnitude = k * (magnitude - restLength);

        // Calcula la fuerza final y la aplica
        force.normalize();
        force *= -magnitude;
        particle->addForce(force);
    }
};