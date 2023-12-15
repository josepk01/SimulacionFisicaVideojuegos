#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include "SolidManager.h"
#include "ParticleSystem.h"
using namespace physx;

class SolidGenerator {
private:
    PxPhysics* gPhysics;
    PxScene* gScene;
    SolidManager& solidManager; // Agrega esta línea
    ParticleSystem* particleS = new ParticleSystem();

public:
    SolidGenerator(PxPhysics* physics, PxScene* scene, SolidManager& manager, ParticleSystem* particleSystem)
        : gPhysics(physics), gScene(scene), solidManager(manager), particleS(particleSystem){}

    void createSolid(const Vector3& position, float mass) {
        // Crear y configurar un nuevo sólido rígido dinámico
        PxRigidDynamic* new_solid = gPhysics->createRigidDynamic(PxTransform({ position.x, position.y, position.z }));
        new_solid->setLinearVelocity({ 0, 5, 0 });  // Puedes modificar esto según sea necesario
        new_solid->setAngularVelocity({ 0, 0, 0 }); // Puedes modificar esto según sea necesario
        PxShape* shape_ad = CreateShape(PxBoxGeometry(5, 5, 5));
        new_solid->attachShape(*shape_ad);
        PxRigidBodyExt::updateMassAndInertia(*new_solid, mass);
        gScene->addActor(*new_solid);

        // Pintar actor dinámico
        RenderItem* dynamic_item = new RenderItem(shape_ad, new_solid, { 0.8, 0.8, 0.8, 1 });
        solidManager.addSolid(new_solid);
        particleS->addActor(new_solid);
    }
};
