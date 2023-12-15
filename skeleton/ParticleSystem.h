#pragma once
#include <map>
#include <list>
#include "ParticleGenerator.h"
#include "ForceGenerator.h"
#include "ExplosionForceGenerator.h"


class ParticleSystem {
private:
    std::list<Particle*> particles;  // Lista de partículas
    std::list<ForceGenerator*> forceGenerators;  // Lista de fuerzas
    std::list<ParticleGenerator*> particle_generators;  // Generadores de partículas
    std::list<Firework*> fireworks;
    std::map<std::string, ForceGenerator*> activeForceGenerators;
    std::list<PxRigidDynamic*> rigidDynamics;  // Lista para sólidos rígidos dinámicos de PhysX
    ExplosionForceGenerator* explosionGen = nullptr;

public:
    ParticleSystem();
    ~ParticleSystem();
    void addForceGenerator(ForceGenerator* forceGenerator);

    void addExplosionGenerator(ExplosionForceGenerator* forceGenerator);

    void integrate(double t);
    ParticleGenerator* getParticleGenerator(std::string name);
    ForceGenerator* getForceGenerator(std::string name);
    void addActor(PxRigidDynamic* actor);
    void addParticleGenerator(ParticleGenerator* generator);
    void createParticleUsingGenerator(std::string generatorName);
    void createParticle(Vector3 position, Vector3 velocity, Vector3 acceleration, double damping, double mass, bool firework);
    void updateParticles(double t);
    void detonateExplosion(float time) {
        if (explosionGen) {
            explosionGen->detonate(time);
        }
    }
    void addParticle(Particle* p);
    void applyForceToParticle(Particle* p, const Vector3& force);
    void activateForceGenerator(const std::string& name);
    void deactivateForceGenerator(const std::string& name);
    std::list<PxRigidDynamic*>& getActors() { return rigidDynamics; }
    std::list<ForceGenerator*>& getForceGenerators() { return forceGenerators; }
};
