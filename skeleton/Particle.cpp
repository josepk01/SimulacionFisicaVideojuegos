#include "particle.h"

// Constructor de Particle
Particle::Particle(Vector3 pos, Vector3 v, Vector3 acceleration, double damping, double mass) {
    this->mass = mass;
    pose = physx::PxTransform(pos);
    vel = v;
    acc = acceleration;
    this->damping = damping;
    renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0)), &pose, Vector4(1, 0, 0, 1));
}

// Destructor de Particle
Particle::~Particle() {
    renderItem->release();
}

// M�todo integrate para actualizar la posici�n y velocidad de la part�cula bas�ndose en su aceleraci�n y damping.
void Particle::integrate(double t) {
    if (t <= 0.0) return; // Evitar la integraci�n si el tiempo es 0 o negativo.

    // Actualizar posici�n con velocidad actual
    pose.p += vel * t;

    // Obtener la fuerza resultante (por ahora solo usamos aceleraci�n)
    Vector3 resultingAcc = acc;

    // Actualizar velocidad con aceleraci�n y damping
    vel += resultingAcc * t;
    vel *= pow(damping, t);
}
