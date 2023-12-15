#pragma once
#include <PxPhysicsAPI.h>
#include "SolidGenerator.h"
#include "RenderUtils.hpp"

using namespace physx;

class SolidGenerator {
private:
    PxPhysics* gPhysics;
    PxScene* gScene;
    float mass;

public:
    SolidGenerator::SolidGenerator(PxPhysics* physics, PxScene* scene, float masa)
        : gPhysics(physics), gScene(scene), mass(masa) {}

    void SolidGenerator::createSolid() {
        // Crear y configurar un nuevo sólido rígido dinámico
        PxRigidDynamic* new_solid = gPhysics->createRigidDynamic(PxTransform({ -70, 200, -70 }));
        new_solid->setLinearVelocity({ 0, 5, 0 });
        new_solid->setAngularVelocity({ 0, 0, 0 });
        PxShape* shape_ad = CreateShape(PxBoxGeometry(5, 5, 5));
        new_solid->attachShape(*shape_ad);
        PxRigidBodyExt::updateMassAndInertia(*new_solid, mass);
        gScene->addActor(*new_solid);

        // Pintar actor dinámico
        RenderItem* dynamic_item = new RenderItem(shape_ad, new_solid, { 0.8, 0.8, 0.8, 1 });
    }
};



