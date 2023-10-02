#include "RenderUtils.hpp"

class Particle {
private:
    physx::PxTransform position;
    
    Vector3 velocity;
    RenderItem* renderItem;

public:
    Particle(Vector3 initialPosition, Vector3 initialVelocity) : position(initialPosition), velocity(initialVelocity), renderItem(nullptr) {
        // Crea el objeto RenderItem y reg�stralo en la escena.
        renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0)),&position,Vector4(1,0,0,1));
        RegisterRenderItem(renderItem); // Utiliza la funci�n de RenderUtils.
    }

    ~Particle() {
        // Deregistra el objeto RenderItem de la escena y libera la memoria.
        if (renderItem != nullptr) {
            DeregisterRenderItem(renderItem); // Utiliza la funci�n de RenderUtils.
            delete renderItem;
        }
    }

    // M�todo para actualizar la posici�n de la part�cula utilizando la integraci�n.
    void Integrate(double t);


};
