#ifndef FIREWORK_H
#define FIREWORK_H
#pragma once

#include "particle.h"
#include <random>
#include <list>

class Firework : public Particle {
public:
    Firework(Vector3 pos);
    ~Firework();

    void explode(std::list<Firework*>& fireworks);

private:
    Vector3 initialVelocity;
    double size;
    Vector4 color;  // Usamos Vector4 para almacenar el color RGBA (con transparencia)
    int shape;     // Podemos usar un int o enum para diferentes formas

    static double randomDouble(double min, double max);
};

#endif // FIREWORK_H
