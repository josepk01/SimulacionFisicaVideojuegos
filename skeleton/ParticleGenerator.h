#pragma once
#include <list>
#include "Particle.h"

class ParticleGenerator {
protected:
    std::string _name;
    Vector3 position;
    Vector3 velocity;
    double rate;

public:
    ParticleGenerator(std::string name, Vector3 position, Vector3 velocity, double rate)
        : _name(name), position(position), velocity(velocity), rate(rate) {}

    virtual void generate(std::list<Particle*>& particles, double t) = 0; // Hacemos esta función puramente virtual

    std::string getName() const {
        return _name;
    }
};
