#pragma once
#include "ForceGenerator.h"

class VortexForceGenerator : public ForceGenerator {
private:
    Vector3 center; // Centro del torbellino
    float K; // Constante de fuerza del torbellino

public:
    VortexForceGenerator(std::string name, const Vector3& center, float K)
        : ForceGenerator(name), center(center), K(K) {}

    void updateForce(Particle* particle, PxRigidDynamic* actor, float duration) override {
        Vector3 position;
        Vector3 velocity;

        // Determinar si estamos tratando con una partícula o un actor físico
        if (particle) {
            position = particle->getPosition();
            velocity = particle->getVelocity();
        }
        if (actor) {
            PxTransform pose = actor->getGlobalPose();
            PxVec3 px_velocity = actor->getLinearVelocity();

            position = Vector3(pose.p.x, pose.p.y, pose.p.z);
            velocity = Vector3(px_velocity.x, px_velocity.y, px_velocity.z);
        }

        Vector3 toCenter = position - center;

        // Asegurarse de que no estemos en el centro para evitar una división por cero
        float distance = toCenter.magnitude();
        if (distance < 0.1f) return; // Suponemos que cerca del centro no hay fuerza de viento

        Vector3 windVelocity = K * Vector3(-toCenter.y, toCenter.x, 0) / distance; // Fuerza tangencial

        // Aplicar la fuerza
        if (particle) {
            particle->addForce(windVelocity - velocity);
        }
        if (actor) {
            PxVec3 force = PxVec3(windVelocity.x - velocity.x, windVelocity.y - velocity.y, windVelocity.z - velocity.z);
            actor->addForce(force, PxForceMode::eFORCE);
        }
    }

};