#pragma once
#include <unordered_map>
#include <PxPhysicsAPI.h>
#include <vector>
#include "RenderUtils.hpp"

using namespace physx;

class SolidManager {
private:
    std::vector<PxRigidDynamic*> solids;
    std::unordered_map<PxRigidDynamic*, Vector3> forceAccumulators;
    PxScene* gScene;

public:
    SolidManager(PxScene* scene) : gScene(scene) {}

    void addSolid(PxRigidDynamic* solid) {
        solids.push_back(solid);
        forceAccumulators[solid] = Vector3(0, 0, 0); // Inicializa el acumulador de fuerzas
    }

    void addForce(PxRigidDynamic* solid, const Vector3& force) {
        if (forceAccumulators.find(solid) != forceAccumulators.end()) {
            forceAccumulators[solid] += force;
        }
    }

    void integrate(double t) {
        for (auto& solid : solids) {
            if (!solid) continue;

            Vector3 accumulatedForce = forceAccumulators[solid];
            // Aplica la fuerza acumulada
            solid->addForce(PxVec3(accumulatedForce.x, accumulatedForce.y, accumulatedForce.z), PxForceMode::eFORCE);

            // Resetea el acumulador de fuerzas
            forceAccumulators[solid] = Vector3(0, 0, 0);
        }
    }

    // Otros métodos según sea necesario...
};
