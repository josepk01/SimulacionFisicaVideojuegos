#include "Particle.h"
#include "PxPhysicsAPI.h"
#include "Render/Camera.h"
#include <glut.h>

Particle::Particle(const physx::PxVec3& position, const physx::PxVec3& velocity)
    : mPosition(position), mVelocity(velocity) {}

void Particle::update(float deltaTime) {
    // Actualiza la posición de la partícula en función de su velocidad constante
    mPosition += mVelocity * deltaTime;
}

void Particle::ParticleRender(const Snippets::Camera& camera) {
    // Supongamos que tienes una función glBegin glEnd para dibujar un punto en OpenGL
    // Ajusta el tamaño del punto y el color según tus necesidades
    glPointSize(200.0f);  // Tamaño del punto
    glColor3f(1.0f, 0.0f, 0.0f);  // Color (en este caso, rojo)

    // Calcula la posición de la partícula en la pantalla utilizando la cámara
    physx::PxVec3 screenPosition = camera.getTransform().transform(mPosition);

    // Dibuja un punto en la posición de la partícula
    glBegin(GL_POINTS);
    glVertex3f(screenPosition.x, screenPosition.y, screenPosition.z);
    glEnd();
}
