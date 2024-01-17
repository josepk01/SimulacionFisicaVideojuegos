#pragma once
#include <unordered_map>
#include <PxPhysicsAPI.h>
#include <vector>
#include "RenderUtils.hpp"

using namespace physx;

class SolidManager {
private:
    std::vector<PxRigidDynamic*> solids;
    Vector3 forceAccumulators;
    PxScene* gScene;
    bool live = true;
    bool perdida = false;
    bool buena = false;

public:
    SolidManager(PxScene* scene) : gScene(scene) {}
    SolidManager::~SolidManager() {
        for (auto* solid : solids) {
            if (solid) {
                solid->release(); // Asumiendo que 'release' es un método para liberar un sólido
            }
        }
        solids.clear();
    }

    void addSolid(PxRigidDynamic* solid, bool es = false) {
        buena = es;
        solids.push_back(solid);
        forceAccumulators = Vector3(0, 0, 0); // Inicializa el acumulador de fuerzas
    }

    void addForce(PxRigidDynamic* solid, const Vector3& force) {
        forceAccumulators += force;
    }

    void SolidManager::integrate(double t) {
        for (auto& solid : solids) {
            if (!solid) continue;

            // Definir la velocidad deseada. Ejemplo: movimiento hacia la cámara.
            Vector3 desiredVelocity = Vector3(0, 0, -50.f); // Ajustar según necesidad

            // Obtener la velocidad actual del sólido
            PxVec3 currentVelocity = solid->getLinearVelocity();
            Vector3 velocityChange = desiredVelocity - Vector3(currentVelocity.x, currentVelocity.y, currentVelocity.z);

            // Calcular la nueva velocidad. Puedes ajustar esto para suavizar la transición si es necesario.
            Vector3 newVelocity = Vector3(currentVelocity.x, currentVelocity.y, currentVelocity.z) + velocityChange * t;

            if (solid->getGlobalPose().p.y == 15 || solid->getGlobalPose().p.y == 0)
                newVelocity.y = -newVelocity.y;
            if (solid->getGlobalPose().p.x == 40 || solid->getGlobalPose().p.x == -40)
                newVelocity.x = -newVelocity.x;
            if (solid->getGlobalPose().p.x == -1100)
                perdida = false;

            // Establecer la nueva velocidad del sólido
            solid->setLinearVelocity(PxVec3(newVelocity.x, newVelocity.y, newVelocity.z));
        }
    }

    void hit()
    {
        live = false;
    }
     auto getSolids()
    {
        return solids;
    }
     bool obj()
     {
         return buena;
     }
};
