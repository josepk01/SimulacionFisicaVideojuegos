#include "ParticleSystem.h"


ParticleSystem::ParticleSystem() {

}

void ParticleSystem::integrate(double t) {
    // Actualiza todas las partículas en la lista de partículas
    for (Particle* p : particles) {
        p->integrate(t);
    }
    for (Particle* f : fireworks) {
        f->integrate(t);
    }
    auto it = fireworks.begin(); 
    while (it != fireworks.end()) {
        if ((*it)->isAlivef()) {
            delete (*it);  
            it = fireworks.erase(it);
        }
        else it++;
    }
}

ParticleGenerator* ParticleSystem::getParticleGenerator(std::string name) {
    // Devuelve un generador de partículas por nombre (si existe)
    for (ParticleGenerator* pg : particle_generators) {
        if (pg->getName() == name) {
            return pg;
        }
    }
    return nullptr;
}

void ParticleSystem::addParticleGenerator(ParticleGenerator* generator) {
    particle_generators.push_back(generator);
}

void ParticleSystem::createParticleUsingGenerator(std::string generatorName) {
    ParticleGenerator* generator = getParticleGenerator(generatorName);
    if (generator) {
        generator->generate(particles, 1.0);  // Suponiendo que 't' es 1.0, ajuste según sea necesario
    }
}

void ParticleSystem::createParticle(Vector3 position, Vector3 velocity, Vector3 acceleration, double damping, double mass, bool firework) {
    if (!firework)
    {
        Particle* newParticle = new Particle(position, velocity, acceleration, damping, mass, 1, firework);
        particles.push_back(newParticle);
    }
    else
    {
        fireworks.push_back(new Firework(position, fireworks));
    }
}

void ParticleSystem::updateParticles(double t) {
    for (ParticleGenerator* pg : particle_generators) {
        pg->generate(particles, t);
    }
}
