#pragma once
#include "ParticleSystem.h"
#include "SolidManager.h"
#include "SolidGenerator.h"

class CollisionManager {
public:
    static bool checkCollisions(ParticleSystem* particleSystem, SolidManager* solidManager) {
        auto particles = particleSystem->getParticles();
        auto solids = solidManager->getSolids();

        for (auto& particle : particles) {
            for (auto& solid : solids) {
                if (isColliding(particle, solid, solidManager)) {
                    particle->isDeadf();
                    solidManager->hit();
                    return true; // Retorna true si hay una colisión
                }
            }
        }
        return false; // Retorna false si no hay colisiones
    }


private:
static bool isColliding(Particle* particle, PxRigidDynamic* solid, SolidManager* solidManager) {
    Vector3 distanceVec = particle->getPosition() - Vector3(solid->getGlobalPose().p.x, solid->getGlobalPose().p.y, solid->getGlobalPose().p.z);
    float distance = distanceVec.magnitude();

    float particleRadius = particle->getRadius();
    float solidSize = SolidGenerator::getApproximateSize(); // Asumiendo que esta función está disponible en SolidGenerator

    float sumRadii = particleRadius + solidSize / 2.0f; // Asumiendo que solidSize es una dimensión de una caja
    if (particle->obj() && solidManager->obj())
        return distance < sumRadii; // Retorna true si están intersectando
    else
        return false;
}

};
