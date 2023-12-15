#pragma once
#include <PxRigidActor.h>
using namespace physx;

class PhysicalObject {
public:
    PhysicalObject(PxRigidActor* actor) : mActor(actor) {}

    PxRigidActor* getActor() const { return mActor; }

    void integrate(double t) {
        // Aquí puedes añadir cualquier lógica que necesites para actualizar el objeto físico
        // Por ejemplo, actualizar la posición si es un objeto dinámico
    }

private:
    PxRigidActor* mActor; // Actor de PhysX
};
