#pragma once

#include <list>
#include "ParticleGenerator.h"



class ParticleSystem {
private:
    std::list<Particle*> particles;  // Lista de partículas
    std::list<ParticleGenerator*> particle_generators;  // Generadores de partículas

public:
    ParticleSystem();

    void integrate(double t);
    ParticleGenerator* getParticleGenerator(std::string name);
    void addParticleGenerator(ParticleGenerator* generator);
    void createParticleUsingGenerator(std::string generatorName);
    void createParticle(Vector3 position, Vector3 velocity, Vector3 acceleration, double damping, double mass, bool firework);
    void updateParticles(double t);
};
