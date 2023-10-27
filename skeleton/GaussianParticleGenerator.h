#pragma once
#include <random> // Para las distribuciones
#include "ParticleGenerator.h"

class GaussianParticleGenerator : public ParticleGenerator {
private:
    double standard_deviation;  // Desviación estándar para la distribución gaussiana
    std::default_random_engine generator; // Motor de generación aleatoria

public:
    GaussianParticleGenerator(std::string name, Vector3 pos, Vector3 vel, double rate, double std_dev)
        : ParticleGenerator(name, pos, vel, rate), standard_deviation(std_dev), generator(std::random_device{}()) {}

    void generate(std::list<Particle*>& particles, double t) override {
        std::normal_distribution<double> distribution_x(velocity.x, standard_deviation);
        std::normal_distribution<double> distribution_y(velocity.y, standard_deviation);
        std::normal_distribution<double> distribution_z(velocity.z, standard_deviation);

        int numParticlesToGenerate = static_cast<int>(rate * t);
        for (int i = 0; i < numParticlesToGenerate; ++i) {
            Vector3 newVelocity(distribution_x(generator), distribution_y(generator), distribution_z(generator));
            Particle* newParticle = new Particle(position, newVelocity, Vector3(0, -9.81, 0), 0.99, 136.32,2,false);
            particles.push_back(newParticle);
        }
    }
};
