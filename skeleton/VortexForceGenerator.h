#pragma once
#include "ForceGenerator.h"

class VortexForceGenerator : public ForceGenerator {
private:
    Vector3 center; // Centro del torbellino (x_c, y_c, 0) para el ejemplo 2D
    float K; // Constante de fuerza del torbellino

public:
    VortexForceGenerator(const Vector3& center, float K)
        : center(center), K(K) {}

    void updateForce(Particle* particle, float duration) override {
        // Solo aplicamos la fuerza si la partícula tiene masa finita

        Vector3 position = particle->getPosition();
        Vector3 toParticle = position - center;

        // Asegurarse de que no estemos en el centro para evitar una división por cero
        float distance = toParticle.magnitude();
        if (distance < 0.1f) return; // Suponemos que cerca del centro no hay fuerza de viento

        Vector3 windVelocity = K * Vector3(-toParticle.y, toParticle.x, 0) / distance; // Fuerza tangencial

        // Aplicamos la fuerza del viento a la partícula
        particle->addForce(windVelocity - particle->getVelocity());
    }
};