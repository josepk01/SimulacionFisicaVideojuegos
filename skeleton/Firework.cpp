#include "Firework.h"
#include <iostream>
Firework::Firework(Vector3 pos, std::list<Firework*>& fireworks, int numero)
    : Particle(pos, Vector3(randomDouble(-50, 50), randomDouble(10, 100), randomDouble(-50, 50)), Vector3(0, -9.81, 0), 0.99, 136.32, 2, true) {

    this->initialVelocity = Vector3(randomDouble(-15, 15), randomDouble(10, 20), randomDouble(-15, 15));
    this->size = randomDouble(0.5, 3.0);
    this->color = Vector4(randomDouble(0, 1), randomDouble(0, 1), randomDouble(0, 1), randomDouble(0.5, 1)); // RGBA
    this->shape = rand() % 3; // Por ahora, tres formas posibles: 0, 1, o 2
    position = pos;
    renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0)), &pose, color);
    fireworkslist = &fireworks;
    num = numero;
    //std::cout << "se crea";
}

double Firework::randomDouble(double min, double max) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(min, max);
    return dist(mt);
}

void Firework::explode() {
    // Cuando la partícula "muere", crea nuevas partículas
    int numNewParticles = rand() % 10 + 5;  // Crea entre 5 y 15 nuevas partículas
    //std::cout << "entra en explotar";
    for (int i = 0; i < numNewParticles; ++i) {
        if (num <= 0)
        {
            isAlive = false;
        }
        else {
            fireworkslist->push_back(new Firework(Particle::pose.p, *fireworkslist, num--));
        }
        // Las nuevas partículas heredan la posición de la partícula original y tienen un tiempo de vida aleatorio entre 1 y 3 segundos
    }
}

Firework::~Firework() {
    explode();
    isAlive = false;
}

void Firework::integrate(double t) {
    Particle::integrate(t);

    if (elapsedTime > lifetime || !isInsideBox(pose.p)) {
        explode();
    }
}