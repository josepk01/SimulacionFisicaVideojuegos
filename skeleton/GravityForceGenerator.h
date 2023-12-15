#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator {
    Vector3 gravity; // La aceleración de la gravedad

public:
    GravityForceGenerator(std::string name, const Vector3& gravity)
        : ForceGenerator(name), gravity(gravity) {}

    void updateForce(Particle* particle, PxRigidDynamic* actor, float duration) override {
        particle->addForce(gravity * particle->getMass());
    }
};
