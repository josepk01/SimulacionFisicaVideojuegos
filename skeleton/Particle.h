#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"

class Particle {
public:
    Particle(Vector3 pos, Vector3 v, Vector3 acceleration, double damping, double mass); 
    ~Particle();

    void integrate(double t);
private:
    double mass;  
    Vector3 vel;
    Vector3 acc;
    double damping;
    physx::PxTransform pose;
    RenderItem* renderItem;
};
