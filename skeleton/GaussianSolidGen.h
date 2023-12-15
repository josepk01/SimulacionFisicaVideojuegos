#pragma once
#include <random>
#include <PxPhysicsAPI.h>
#include "SolidGenerator.h"
#include "RenderUtils.hpp"

using namespace physx;

class GaussianSolidGenerator {
private:
    PxPhysics* gPhysics;
    PxScene* gScene;
    std::default_random_engine generator;
    Vector3 position;
    double standard_deviation;
    float minMass;
    float maxMass;
    SolidGenerator solidGenerator;

public:
    GaussianSolidGenerator(PxPhysics* physics, PxScene* scene, SolidManager& manager, Vector3 pos, double std_dev, float minMass, float maxMass)
        : gPhysics(physics), gScene(scene), solidGenerator(physics, scene, manager), position(pos), standard_deviation(std_dev), minMass(minMass), maxMass(maxMass), generator(std::random_device{}()) {}

    void createSolid() {
        std::normal_distribution<double> distribution(0.0, standard_deviation);
        std::uniform_real_distribution<float> massDistribution(minMass, maxMass);

        // Crear y configurar un nuevo sólido rígido dinámico
        Vector3 velocity(distribution(generator), distribution(generator), distribution(generator));
        float mass = massDistribution(generator);

        solidGenerator.createSolid(position, mass);
    }
};
