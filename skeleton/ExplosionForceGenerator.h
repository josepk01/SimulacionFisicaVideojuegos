#pragma once
#include "ForceGenerator.h"

class ExplosionForceGenerator : public ForceGenerator {
private:
    Vector3 center; // Centro de la explosión (x_c, y_c, z_c)
    float K; // Intensidad de la explosión
    float R; // Radio de la explosión
    float tau; // Constante de tiempo de la explosión
    float startTime; // Momento en que ocurre la explosión

public:
    ExplosionForceGenerator(const Vector3& center, float K, float R, float tau)
        : center(center), K(K), R(R), tau(tau), startTime(0) {}

    void updateForce(Particle* particle, float time) override {

        Vector3 toParticle = particle->getPosition() - center;
        float distance = toParticle.magnitude();

        // Si la partícula está dentro del radio de la explosión
        if (distance < R) {
            float timeSinceExplosion = time - startTime;
            // Asegúrate de que no dividimos por cero
            if (distance < 0.001f) distance = 0.001f;
            // Calcula la fuerza de la explosión
            float magnitude = K * exp(-timeSinceExplosion / tau) / (distance * distance);
            toParticle.normalize();
            particle->addForce(toParticle * magnitude * 5000);
        }
    }

    // Método para detonar la explosión
    void detonate(float time) {
        startTime = time;
    }
};
