#pragma once
#include <PxRigidActor.h>
using namespace physx;

class PhysicalObject {
public:
    PhysicalObject(PxRigidActor* actor) : mActor(actor) {}

    PxRigidActor* getActor() const { return mActor; }

    void integrate(double t) {
        // Aqu� puedes a�adir cualquier l�gica que necesites para actualizar el objeto f�sico
        // Por ejemplo, actualizar la posici�n si es un objeto din�mico
    }

private:
    PxRigidActor* mActor; // Actor de PhysX
};
