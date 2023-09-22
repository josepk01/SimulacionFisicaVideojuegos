#include "Particle.h"
#include "PxPhysicsAPI.h"
#include "Render/Camera.h"
#include <glut.h>

Particle::Particle(const physx::PxVec3& position, const physx::PxVec3& velocity)
    : mPosition(position), mVelocity(velocity) {}

void Particle::update(float deltaTime) {
    // Actualiza la posici�n de la part�cula en funci�n de su velocidad constante
    mPosition += mVelocity * deltaTime;
}

void Particle::ParticleRender(const Snippets::Camera& camera) {
    // Supongamos que tienes una funci�n glBegin glEnd para dibujar un punto en OpenGL
    // Ajusta el tama�o del punto y el color seg�n tus necesidades
    glPointSize(200.0f);  // Tama�o del punto
    glColor3f(1.0f, 0.0f, 0.0f);  // Color (en este caso, rojo)

    // Calcula la posici�n de la part�cula en la pantalla utilizando la c�mara
    physx::PxVec3 screenPosition = camera.getTransform().transform(mPosition);

    // Dibuja un punto en la posici�n de la part�cula
    glBegin(GL_POINTS);
    glVertex3f(screenPosition.x, screenPosition.y, screenPosition.z);
    glEnd();
}
