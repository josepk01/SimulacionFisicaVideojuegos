#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator {
    Vector3 gravity; // La aceleración de la gravedad

public:
    GravityForceGenerator(const Vector3& gravity) : gravity(gravity) {}

    void updateForce(Particle* particle, float duration) override {
        particle->addForce(gravity * particle->getMass());
    }
};