#include "foundation/PxVec3.h"
#include "PxPhysicsAPI.h"

// Declaración forward de la clase Camera (puedes necesitarla en Particle)
namespace Snippets {
    class Camera;
}

class Particle {
public:
    Particle(const physx::PxVec3& position, const physx::PxVec3& velocity);

    void update(float deltaTime);
    void ParticleRender(const Snippets::Camera& camera);

private:
    physx::PxVec3 mPosition;  // Posición actual de la partícula
    physx::PxVec3 mVelocity;  // Velocidad constante de la partícula
};