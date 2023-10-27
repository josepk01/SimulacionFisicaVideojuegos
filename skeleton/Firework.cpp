#include "Firework.h"
Firework::Firework(Vector3 pos)
    : Particle(pos, Vector3(randomDouble(-50, 50), randomDouble(10, 100), randomDouble(-50, 50)), Vector3(0, -9.81, 0), 0.99, 1.0, 10, true) {

    this->initialVelocity = Vector3(randomDouble(-50, 50), randomDouble(10, 100), randomDouble(-50, 50));
    this->size = randomDouble(0.5, 3.0);
    this->color = Vector4(randomDouble(0, 1), randomDouble(0, 1), randomDouble(0, 1), randomDouble(0.5, 1)); // RGBA
    this->shape = rand() % 3; // Por ahora, tres formas posibles: 0, 1, o 2
}

double Firework::randomDouble(double min, double max) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(min, max);
    return dist(mt);
}

void Firework::explode(std::list<Firework*>& fireworks) {
    // Cuando la partícula "muere", crea nuevas partículas
    int numNewParticles = rand() % 10 + 5;  // Crea entre 5 y 15 nuevas partículas
    for (int i = 0; i < numNewParticles; ++i) {
        fireworks.push_back(new Firework(this->pose.p)); // Las nuevas partículas heredan la posición de la partícula original y tienen un tiempo de vida aleatorio entre 1 y 3 segundos
    }
}

Firework::~Firework() {
}
