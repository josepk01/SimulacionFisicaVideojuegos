#pragma once
#include "ForceGenerator.h"
#include "Particle.h"

class SpringForceGenerator : public ForceGenerator {
private:
    Vector3 anchor;  // Punto de anclaje del muelle
    float k;         // Constante de muelle
    float restLength;// Longitud de reposo

public:
    SpringForceGenerator(const std::string& name, const Vector3& anchor, float k, float restLength)
        : ForceGenerator(name), anchor(anchor), k(k), restLength(restLength) {}

    void updateForce(Particle* particle, float duration) override {
        Vector3 force;
        force = particle->getPosition() - anchor; // Vector de la partícula al anclaje
        float magnitude = force.magnitude();
        magnitude = fabs(magnitude - restLength);
        magnitude *= k;

        force.normalize();
        force *= -magnitude;
        particle->addForce(force);
    }
    void setSpringConstant(const int diff )
    {

    }
};
