#pragma once
#include "ForceGenerator.h"
#include "Particle.h"

class SpringForceGenerator : public ForceGenerator {
private:
    Particle* mParticle;  // Partícula asociada
    Vector3 anchor;       // Punto de anclaje del muelle
    float k;              // Constante de muelle
    float restLength;     // Longitud de reposo

public:
    SpringForceGenerator(const std::string& name, Particle* particle, const Vector3& anchor, float k0, float restLength)
        : ForceGenerator(name), mParticle(particle), anchor(anchor), k(k0), restLength(restLength) {}

    void updateForce(Particle* particle, PxRigidDynamic* actor, float duration) override {
        if (!mParticle) return; // Verifica que la partícula exista

        Vector3 force = mParticle->getPosition() - anchor; // Vector de la partícula al anclaje
        float magnitude = force.magnitude();
        magnitude = fabs(magnitude - restLength);
        magnitude *= k;

        force.normalize();
        force *= -magnitude;
        mParticle->addForce(force);
    }
    void setSpringConstant(int newK) {

        k = newK;
    }
};
