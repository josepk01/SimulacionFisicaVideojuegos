#pragma once
#include "ForceGenerator.h"

class ExplosionForceGenerator : public ForceGenerator {
private:
    Vector3 center; // Centro de la explosi�n
    float K; // Intensidad de la explosi�n
    float R; // Radio de la explosi�n
    float tau; // Constante de tiempo de la explosi�n
    float startTime = 0; // Momento en que ocurre la explosi�n

public:
    ExplosionForceGenerator(std::string name, const Vector3& center, float K, float R, float tau)
        : ForceGenerator(name), center(center), K(K), R(R), tau(tau), startTime(0) {}

    void updateForce(Particle* particle, float time) override {

        Vector3 toParticle = particle->getPosition() - center;
        float distance = toParticle.magnitude();

        // Si la part�cula est� dentro del radio de la explosi�n
        if (distance < R && startTime >0) {
            float timeSinceExplosion = time - startTime;
            // Aseg�rate de que no dividimos por cero
            if (distance < 0.001f) distance = 0.001f;
            // Calcula la fuerza de la explosi�n
            float magnitude = K * exp(-timeSinceExplosion / tau) / (distance * distance);
            toParticle.normalize();
            particle->addForce(toParticle * magnitude * 5000);
            if (time >= startTime + 4)
                startTime = 0;
        }
    }

    // M�todo para detonar la explosi�n
    void detonate(float time) {
        startTime = time;
    }
};
