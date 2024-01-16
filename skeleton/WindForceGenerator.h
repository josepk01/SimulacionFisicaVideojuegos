#pragma once
#include "ForceGenerator.h"
#include <cmath> // Para funciones matemáticas

class WindForceGenerator : public ForceGenerator {
private:
    Vector3 windVelocity; // La velocidad del viento
    float k1; // Coeficiente de arrastre lineal
    float changeFrequency; // Frecuencia con la que cambia el viento (para viento variable)
    float timeSinceLastChange; // Tiempo transcurrido desde el último cambio

public:
    WindForceGenerator(std::string name, const Vector3& windVelocity, float k1, float changeFrequency = 5.0f)
        : ForceGenerator(name), windVelocity(windVelocity), k1(k1), changeFrequency(changeFrequency), timeSinceLastChange(100) {}

    void updateForce(Particle* particle, PxRigidDynamic* actor, float duration) override {
        std::string nameF = getName();
        if (nameF == "VariableWind") {
            // Actualiza el tiempo transcurrido
            timeSinceLastChange += duration;
            if (timeSinceLastChange >= changeFrequency) {
                // Cambia la dirección y velocidad del viento
                // Por ejemplo, puedes usar una función seno y coseno para un cambio suave
                float angle = static_cast<float>(rand()) / RAND_MAX * 2.0f * 3.1415;
                float speed = static_cast<float>(rand()) / RAND_MAX * 10.0f; // Velocidad máxima de 10, por ejemplo
                windVelocity = Vector3(std::cos(angle) * speed, 0, std::sin(angle) * speed);

                // Restablece el temporizador
                timeSinceLastChange = 0;
            }
        }

        Vector3 velocity;
        if (particle) {
            velocity = particle->getVelocity();
        }
        else if (actor) {
            PxVec3 px_velocity = actor->getLinearVelocity();
            velocity = Vector3(px_velocity.x, px_velocity.y, px_velocity.z);
        }
        // Calcular la velocidad relativa entre el viento y la partícula o actor
        Vector3 relativeVel = windVelocity - velocity;
        // Aplicar la fuerza del viento proporcional a la diferencia de velocidad
        Vector3 force = k1 * relativeVel;

        if (particle) {
            particle->addForce(force);
        }
        else if (actor) {
            actor->addForce(PxVec3(force.x, force.y, force.z), PxForceMode::eFORCE);
        }
    }
};
