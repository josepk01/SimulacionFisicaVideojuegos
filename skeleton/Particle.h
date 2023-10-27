#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include "Render/Camera.h"
#include <list>

class Firework;

class Particle {
public:
    Particle(Vector3 pos, Vector3 v, Vector3 acceleration, double damping, double mass, double lifetime, bool firework);
    virtual ~Particle();

    void integrate(double t);
    virtual void explode(std::list<Firework*> fireworks) {}
    bool isAlivef()
    {
        return isAlive;
    }
protected:
    bool isInsideBox(const Vector3& point);
    bool isAlive;  // Verdadero si la partícula todavía está activa.
    bool fire;
    double mass;
    Vector3 vel;
    Vector3 acc;
    double damping;
    double lifetime;
    double elapsedTime;
    physx::PxTransform pose;
    RenderItem* renderItem;
};