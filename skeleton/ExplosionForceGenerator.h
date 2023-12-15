#pragma once
#include "ForceGenerator.h"
#include "RenderUtils.hpp"

using namespace physx;

class ExplosionForceGenerator : public ForceGenerator {
private:
    Vector3 center; // Centro de la explosión
    float K; // Intensidad de la explosión
    float R; // Radio de la explosión
    float tau; // Constante de tiempo de la explosión
    float startTime = 0; // Momento en que ocurre la explosión
    RenderItem* explosionSphere = nullptr; // Esfera para visualizar la explosión

public:
    ExplosionForceGenerator(std::string name, const Vector3& center, float K, float R, float tau)
        : ForceGenerator(name), center(center), K(K), R(R), tau(tau), startTime(0), explosionSphere(nullptr) {}

    ~ExplosionForceGenerator() {
        if (explosionSphere) {
            delete explosionSphere; // Limpiar el RenderItem
        }
    }

    void updateForce(Particle* particle, PxRigidDynamic* actor, float time) override {
        Vector3 position;
        if (particle) {
            position = particle->getPosition();
        }
        else if (actor) {
            PxTransform pose = actor->getGlobalPose();
            position = Vector3(pose.p.x, pose.p.y, pose.p.z);
        }

        Vector3 toCenter = position - center;
        float distance = toCenter.magnitude();

        // Si la partícula o actor está dentro del radio de la explosión
        if (distance < R && startTime > 0) {
            float timeSinceExplosion = time - startTime;
            if (distance < 0.001f) distance = 0.001f;
            float magnitude = K * exp(-timeSinceExplosion / tau) / (distance * distance);
            toCenter.normalize();

            Vector3 explosionForce = toCenter * magnitude * 5000;

            if (particle) {
                particle->addForce(explosionForce);
            }
            if (actor) {
                actor->addForce(PxVec3(explosionForce.x, explosionForce.y, explosionForce.z), PxForceMode::eFORCE);
            }

            if (time >= startTime + 4)
                startTime = 0;
        }
    }

    void detonate(float time) {
        startTime = time;
    }
};
