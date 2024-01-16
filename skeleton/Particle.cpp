#include "particle.h"


// Constructor de Particle
Particle::Particle(Vector3 pos, Vector3 v, Vector3 acceleration, double damping, double mass, double lifetime, bool firework)
    : pose(pos), vel(v), acc(acceleration), damping(damping), mass(mass),
    lifetime(lifetime), fire(firework), elapsedTime(0.0), isAlive(true), forceAccum(Vector3(0, 0, 0)) {
    if (mass == 28.5) {  // Glock
        renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(0.5)), &pose, Vector4(1, 0, 0, 1));  // Rojo
    }
    else if (mass == 30.1) {  // AK-47
        renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(0.6)), &pose, Vector4(0, 1, 0, 1));  // Verde
    }
    else if (mass == 36.3) {  // AWP
        renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(0.7)), &pose, Vector4(0, 0, 1, 1));  // Azul
    }
    else if (mass == 160.3) {  // Cal.50
        renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0)), &pose, Vector4(1, 1, 0, 1));  // Amarillo
    }
    else// el resto de aprticulas
        renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0)), &pose, Vector4(1, 1, 1, 1)); // Negro
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
    float halfSize = 1000.0; // La mitad de 100
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