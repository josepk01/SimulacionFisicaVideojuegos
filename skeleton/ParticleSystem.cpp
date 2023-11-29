#include "ParticleSystem.h"
#include "ForceGenerator.h"


ParticleSystem::ParticleSystem() {

}
ParticleSystem::~ParticleSystem()
{

}
void ParticleSystem::addForceGenerator(ForceGenerator* forceGenerator) {
    forceGenerators.push_back(forceGenerator);
}
ForceGenerator* ParticleSystem::getForceGenerator(std::string name) {
    for (ForceGenerator* fg : forceGenerators) {
        if (fg->getName() == name) {
            return fg;
        }
    }
    return nullptr;
}
void ParticleSystem::integrate(double t) {
    // Actualiza todas las partículas en la lista de partículas
    for (ForceGenerator* fg : forceGenerators) {
        for (Particle* p : particles) {
            fg->updateForce(p, t);
        }
    }

    // Luego integramos las partículas con las nuevas fuerzas aplicadas
    for (Particle* p : particles) {
        p->integrate(t);
    }
    for (Firework* f : fireworks) {
        f->integrate(t);
    }

    // Eliminar partículas que no están vivas
    for (auto it = particles.begin(); it != particles.end(); ) {
        if (!(*it)->isAlivef()) {
            delete* it;
            it = particles.erase(it);
        }
        else {
            ++it;
        }
    }

    for (auto it = fireworks.begin(); it != fireworks.end(); ) {
        if (!(*it)->isAlivef()) {
            delete* it;
            it = fireworks.erase(it);
        }
        else {
            ++it;
        }
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
        Particle* newParticle = new Particle(position, velocity, acceleration, damping, mass, 4, firework);
        particles.push_back(newParticle);
    }
    else
    {
        fireworks.push_back(new Firework(position, fireworks, 3));
    }
}

void ParticleSystem::updateParticles(double t) {
    for (ParticleGenerator* pg : particle_generators) {
        pg->generate(particles, t);
    }
}


