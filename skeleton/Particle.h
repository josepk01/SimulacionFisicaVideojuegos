#pragma once
#include "core.hpp"
#include "RenderUtils.hpp"
#include "Render/Camera.h"
#include <list>
#include <vector>

class Firework;

class Particle {
public:
    Particle(Vector3 pos, Vector3 v, Vector3 acceleration, double damping, double mass, double lifetime, bool firework);
    virtual ~Particle();

    void integrate(double t);
    void addForce(const Vector3& force);

    void clearAccumulator();

    virtual void explode(std::list<Firework*> fireworks) {}
    bool isAlivef()
    {
        return isAlive;
    }
    double getMass()
    {
        return mass;
    }
    Vector3 getVelocity()
    {
        return vel;
    }
    Vector3 getPosition()
    {
        return pose.p;
    }
protected:
    bool isInsideBox(const Vector3& point);
    bool isAlive;  // Verdadero si la partícula todavía está activa.
    bool fire;
    double mass;
    Vector3 vel; // Velocidad actual
    Vector3 acc; // Aceleración actual (se usará para almacenar la acumulación de fuerzas)
    double damping;
    double lifetime;
    double elapsedTime;
    physx::PxTransform pose;
    RenderItem* renderItem;
    Vector3 forceAccum; // Acumulador de fuerzas aplicadas a la partícula
};