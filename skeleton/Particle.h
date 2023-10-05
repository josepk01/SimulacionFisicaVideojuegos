#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"


class Particle {
public:
    Particle(Vector3 pos, Vector3 v);
    ~Particle();

    void integrate(double t);
private:
    Vector3 vel;
    physx::PxTransform pose;
    RenderItem* renderItem;
}
;
