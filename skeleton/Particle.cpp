#include "particle.h"

// Constructor de Particle
Particle::Particle(Vector3 pos, Vector3 v){
    pose = physx::PxTransform(pos);
    vel = v;
    renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0)), &pose, Vector4(1, 0, 0, 1));
}

// Destructor de Particle
Particle::~Particle() {
    renderItem->release();
}

// Método integrate para actualizar la posición de la partícula basándose en su velocidad.
void Particle::integrate(double t) {
    pose.p.x += vel.x * t;
    pose.p.y += vel.y * t;
    pose.p.z += vel.z * t;
}
