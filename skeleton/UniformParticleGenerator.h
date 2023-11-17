#pragma once
#include <random> // Para las distribuciones
#include "ParticleGenerator.h"

class UniformParticleGenerator : public ParticleGenerator {
private:
    double velocity_range;  // Rango de velocidades posibles
    std::default_random_engine generator; // Motor de generación aleatoria

public:
    UniformParticleGenerator(std::string name, Vector3 pos, Vector3 vel, double rate, double vel_range)
        : ParticleGenerator(name, pos, vel, rate), velocity_range(vel_range), generator(std::random_device{}()) {}

    void generate(std::list<Particle*>& particles, double t) override {
        std::uniform_real_distribution<double> distribution_x(velocity.x - velocity_range / 2, velocity.x + velocity_range / 2);
        std::uniform_real_distribution<double> distribution_y(velocity.y - velocity_range / 2, velocity.y + velocity_range / 2);
        std::uniform_real_distribution<double> distribution_z(velocity.z - velocity_range / 2, velocity.z + velocity_range / 2);

        int numParticlesToGenerate = static_cast<int>(rate * t);
        for (int i = 0; i < numParticlesToGenerate; ++i) {
            Vector3 newVelocity(distribution_x(generator), distribution_y(generator), distribution_z(generator));
            Particle* newParticle = new Particle(position, newVelocity, Vector3(0, -9.81, 0), 0.99, 136.32,live,false);
            particles.push_back(newParticle);
        }
    }
};
