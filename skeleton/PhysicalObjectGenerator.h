#pragma once
#include <PxPhysicsAPI.h>
#include <PxRigidStatic.h>
#include "PhysicalObject.h"
using namespace physx;


class PhysicalObjectGenerator {
    std::vector<PhysicalObject*> physicalObjects;
public:
    PhysicalObjectGenerator(PxPhysics* physics, PxScene* scene, PxMaterial* material)
        : mPhysics(physics), mScene(scene), mMaterial(material) {}

    PxRigidStatic* createStaticObject(const PxTransform& transform) {
        PxRigidStatic* staticObject = mPhysics->createRigidStatic(transform);
        PxShape* shape = mPhysics->createShape(PxBoxGeometry(10.0f, 10.0f, 10.0f), *mMaterial);
        staticObject->attachShape(*shape);

        mScene->addActor(*staticObject);
        return staticObject;
    }

    PxRigidDynamic* createDynamicObject(const PxTransform& transform, float density) {
        PxRigidDynamic* dynamicObject = mPhysics->createRigidDynamic(transform);

        PxShape* shape = mPhysics->createShape(PxBoxGeometry(1.0f, 1.0f, 1.0f), *mMaterial);
        dynamicObject->attachShape(*shape);
        PxRigidBodyExt::updateMassAndInertia(*dynamicObject, density);

        mScene->addActor(*dynamicObject);
        return dynamicObject;
    }

    void addPhysicalObject(PhysicalObject* object) {
        physicalObjects.push_back(object);
    }

    void integrateAll(double t) {
        for (auto& object : physicalObjects) {
            object->integrate(t);
        }
    }

    ~PhysicalObjectGenerator() {
        for (auto& object : physicalObjects) {
            delete object;
        }
    }

private:
    PxPhysics* mPhysics;
    PxScene* mScene;
    PxMaterial* mMaterial;
};
