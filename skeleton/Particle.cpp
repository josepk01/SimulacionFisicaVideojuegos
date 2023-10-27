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


// Método integrate para actualizar la posición y velocidad de la partícula basándose en su aceleración y damping.
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


    // Actualizar posición con velocidad actual
    pose.p += vel * t;

    // Obtener la fuerza resultante (por ahora solo usamos aceleración)
    Vector3 resultingAcc = acc;

    // Actualizar velocidad con aceleración y damping
    vel += resultingAcc * t;
    vel *= pow(damping, t);
}