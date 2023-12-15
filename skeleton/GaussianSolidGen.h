#pragma once
#include <random>
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"

using namespace physx;

class GaussianSolidGenerator {
private:
    PxPhysics* gPhysics;
    PxScene* gScene;
    std::default_random_engine generator;
    Vector3 position;
    double standard_deviation;  // Desviación estándar para la distribución gaussiana
    float minMass;
    float maxMass;

public:
    GaussianSolidGenerator(PxPhysics* physics, PxScene* scene, Vector3 pos, double std_dev, float minMass, float maxMass)
        : gPhysics(physics), gScene(scene), position(pos), standard_deviation(std_dev), minMass(minMass), maxMass(maxMass),
        generator(std::random_device{}()) {}

    void createSolid() {
        std::normal_distribution<double> distribution(0.0, standard_deviation);
        std::uniform_real_distribution<float> massDistribution(minMass, maxMass);

        // Crear y configurar un nuevo sólido rígido dinámico
        Vector3 velocity(distribution(generator), distribution(generator), distribution(generator));
        float mass = massDistribution(generator);

        PxRigidDynamic* new_solid = gPhysics->createRigidDynamic(PxTransform(position));
        new_solid->setLinearVelocity(PxVec3(velocity.x, velocity.y, velocity.z));
        new_solid->setAngularVelocity({ 0, 0, 0 });
        PxShape* shape_ad = CreateShape(PxBoxGeometry(5, 5, 5));
        new_solid->attachShape(*shape_ad);
        PxRigidBodyExt::updateMassAndInertia(*new_solid, mass);
        gScene->addActor(*new_solid);

        // Pintar actor dinámico
        RenderItem* dynamic_item = new RenderItem(shape_ad, new_solid, { 0.8, 0.8, 0.8, 1 });
    }
};
