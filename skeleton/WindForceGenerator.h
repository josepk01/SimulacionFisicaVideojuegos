#pragma once
#include "ForceGenerator.h"

class WindForceGenerator : public ForceGenerator {
private:
    Vector3 windVelocity; // La velocidad del viento
    float k1; // Coeficiente de arrastre lineal

public:
    WindForceGenerator(std::string name, const Vector3& windVelocity, float k1)
        : ForceGenerator(name), windVelocity(windVelocity), k1(k1) {}

    void updateForce(Particle* particle, PxRigidDynamic* actor, float duration) override {
        Vector3 velocity;
        if (particle) {
            velocity = particle->getVelocity();
        }
        else if (actor) {
            PxVec3 px_velocity = actor->getLinearVelocity();
            velocity = Vector3(px_velocity.x, px_velocity.y, px_velocity.z);
        }

        // Calcular la velocidad relativa entre el viento y la partícula o actor
        Vector3 relativeVel = windVelocity - velocity;
        // Aplicar la fuerza del viento proporcional a la diferencia de velocidad
        Vector3 force = k1 * relativeVel;

        if (particle) {
            particle->addForce(force);
        }
        else if (actor) {
            actor->addForce(PxVec3(force.x, force.y, force.z), PxForceMode::eFORCE);
        }
    }
};
