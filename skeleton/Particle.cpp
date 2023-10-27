#include "particle.h"
#include "Firework.h"

// Constructor de Particle
Particle::Particle(Vector3 pos, Vector3 v, Vector3 acceleration, double damping, double mass, double lifetime,bool firework) {
    this->mass = mass;
    pose = physx::PxTransform(pos);
    vel = v;
    acc = acceleration;
    this->damping = damping;
    this->lifetime = lifetime;
    this->elapsedTime = 0.0;
    renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0)), &pose, Vector4(1, 0, 0, 1));
    fire = firework;
}

// Destructor de Particle
Particle::~Particle() {
    renderItem->release();
}

bool Particle::isInsideBox(const Vector3& point) {
    float halfSize = 50.0; // La mitad de 100
    return point.x >= pose.p.x - halfSize && point.x <= pose.p.x + halfSize &&
        point.y >= pose.p.y - halfSize && point.y <= pose.p.y + halfSize &&
        point.z >= pose.p.z - halfSize && point.z <= pose.p.z + halfSize;
}


// M�todo integrate para actualizar la posici�n y velocidad de la part�cula bas�ndose en su aceleraci�n y damping.
void Particle::integrate(double t) {
    if (t <= 0.0) return;

    elapsedTime += t;
    if (elapsedTime > lifetime || !isInsideBox(pose.p)) {
        if (fire) {
            explode(fireworks);
        }
        delete this;
        return;
    }


    // Actualizar posici�n con velocidad actual
    pose.p += vel * t;

    // Obtener la fuerza resultante (por ahora solo usamos aceleraci�n)
    Vector3 resultingAcc = acc;

    // Actualizar velocidad con aceleraci�n y damping
    vel += resultingAcc * t;
    vel *= pow(damping, t);
}