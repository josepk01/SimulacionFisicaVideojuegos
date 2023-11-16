#pragma once
#include "ForceGenerator.h"

class WindForceGenerator : public ForceGenerator {
private:
    Vector3 windVelocity; // La velocidad del viento
    float k1; // Coeficiente de arrastre lineal

public:
    WindForceGenerator(const Vector3& windVelocity, float k1)
        : windVelocity(windVelocity), k1(k1) {}

    void updateForce(Particle* particle, float duration) override {
        // Asumimos que k2 es cero como se especifica en la tarea

        // Calcular la velocidad relativa entre el viento y la partícula
        Vector3 relativeVel = windVelocity - particle->getVelocity();
        // Aplicar la fuerza del viento proporcional a la diferencia de velocidad
        Vector3 force = k1 * relativeVel;

        particle->addForce(force);
    }
};
