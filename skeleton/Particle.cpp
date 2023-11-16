#include "particle.h"


// Constructor de Particle
Particle::Particle(Vector3 pos, Vector3 v, Vector3 acceleration, double damping, double mass, double lifetime, bool firework)
    : pose(pos), vel(v), acc(acceleration), damping(damping), mass(mass),
    lifetime(lifetime), fire(firework), elapsedTime(0.0), isAlive(true), forceAccum(Vector3(0, 0, 0)) {
    renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0)), &pose, Vector4(1, 0, 0, 1));
}

// Destructor de Particle
Particle::~Particle() {
    isAlive = false;
    // Asegúrate de liberar correctamente los recursos
    if (renderItem) {
        renderItem->release();
    }
}

bool Particle::isInsideBox(const Vector3& point) {
    float halfSize = 100.0; // La mitad de 100
    return point.x >= pose.p.x - halfSize && point.x <= pose.p.x + halfSize &&
        point.y >= pose.p.y - halfSize && point.y <= pose.p.y + halfSize &&
        point.z >= pose.p.z - halfSize && point.z <= pose.p.z + halfSize;
}

// Método para añadir fuerzas al acumulador
void Particle::addForce(const Vector3& force) {
    forceAccum += force;
}

// Método para limpiar el acumulador de fuerzas
void Particle::clearAccumulator() {
    forceAccum = Vector3(0, 0, 0);
}

// Método integrate para actualizar la posición y velocidad de la partícula
void Particle::integrate(double t) {
    if (t <= 0.0 || !isAlive) return;

    elapsedTime += t;

    // Actualizar posición con velocidad actual
    pose.p += vel * t;

    // Calcular la aceleración resultante a partir del acumulador de fuerzas
    Vector3 resultingAcc = acc;
    if (mass > 0) {
        resultingAcc += forceAccum / mass;
    }

    // Actualizar velocidad con la aceleración resultante y aplicar el damping
    vel += resultingAcc * t;
    vel *= pow(damping, t);

    // Limpiar el acumulador de fuerzas para el siguiente paso
    clearAccumulator();

    // Verificar si la partícula sigue "viva"
    if (elapsedTime > lifetime || !isInsideBox(pose.p)) {
        isAlive = false;
    }
}