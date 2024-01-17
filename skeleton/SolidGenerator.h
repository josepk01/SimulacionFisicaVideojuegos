#pragma once
#include <PxPhysicsAPI.h>
#include "RenderUtils.hpp"
#include "SolidManager.h"
#include "ParticleSystem.h"
using namespace physx;

class SolidGenerator {

public:
    enum class Tipo { SOLIDO };
    SolidGenerator(PxPhysics* physics, PxScene* scene, SolidManager& manager, ParticleSystem* particleSystem, bool poste = false)
        : gPhysics(physics), gScene(scene), solidManager(manager), particleS(particleSystem), posteO(poste){}

    void createSolid(const Vector3& position, float mass) {
        // Crear y configurar un nuevo sólido rígido dinámico
        PxRigidDynamic* new_solid = gPhysics->createRigidDynamic(PxTransform({ position.x, position.y, position.z }));
        new_solid->setLinearVelocity({ 0, 5, 0 });  // Puedes modificar esto según sea necesario
        new_solid->setAngularVelocity({ 0, 0, 0 }); // Puedes modificar esto según sea necesario
        PxShape* shape_ad = CreateShape(PxBoxGeometry(5, 5, 5));
        new_solid->attachShape(*shape_ad);
        PxRigidBodyExt::updateMassAndInertia(*new_solid, mass);
        gScene->addActor(*new_solid);

        // Pintar actor dinámico
        RenderItem* dynamic_item = new RenderItem(shape_ad, new_solid, { 0.8, 0.8, 0.8, 1 });
        solidManager.addSolid(new_solid);
        particleS->addActor(new_solid);
    }
    void createPost(const Vector3& position, const Vector3& dimensions, const Vector4& color) {
        // Crear un actor estático que actúa como poste
        PxRigidStatic* post = gPhysics->createRigidStatic(PxTransform({ position.x, position.y, position.z }));
        PxShape* shape = CreateShape(PxBoxGeometry(dimensions.x, dimensions.y, dimensions.z));
        post->attachShape(*shape);
        gScene->addActor(*post);

        // Pintar el poste
        RenderItem* renderItem = new RenderItem(shape, post, color);
    }
    void createRandomSolid(const Vector3& position, const PxTransform& cameraTransform) {
        // Generar forma y tamaño aleatorios
        float width = static_cast<float>(rand()) / RAND_MAX * 10.0f; // Ancho entre 0 y 10
        float height = static_cast<float>(rand()) / RAND_MAX * 10.0f; // Altura entre 0 y 10
        float depth = static_cast<float>(rand()) / RAND_MAX * 10.0f; // Profundidad entre 0 y 10

        // Crear y configurar un nuevo sólido rígido dinámico
        PxRigidDynamic* new_solid = gPhysics->createRigidDynamic(PxTransform({ position.x, position.y, position.z }));
        PxShape* shape = CreateShape(PxBoxGeometry(width / 2.0f, height / 2.0f, depth / 2.0f));
        new_solid->attachShape(*shape);

        // Establecer masa aleatoria
        float mass = static_cast<float>(rand()) / RAND_MAX * 50.0f; // Masa entre 0 y 50
        PxRigidBodyExt::updateMassAndInertia(*new_solid, mass);

        // Calcular dirección hacia la cámara
        PxVec3 cameraDirection = cameraTransform.q.getBasisVector0().getNormalized();
        float speed = static_cast<float>(rand()) / RAND_MAX * 100.0f; // Velocidad entre 0 y 100
        new_solid->setLinearVelocity(cameraDirection * speed);
        gScene->addActor(*new_solid);

        // Pintar actor dinámico
        RenderItem* dynamic_item = new RenderItem(shape, new_solid, { 0.8, 0.8, 0.8, 1 });
        solidManager.addSolid(new_solid, true);
        particleS->addActor(new_solid);
    }
    void generateRandomSolids(int numberOfSolids, const PxTransform& cameraTransform) {
        for (int i = 0; i < numberOfSolids; ++i) {
            // Generar posición aleatoria en la mitad del suelo
            float x = -50.0f + static_cast<float>(rand()) / RAND_MAX * 100.0f; // X entre -50 y 50
            float y = 10.0f; // Altura fija
            float z = -800.0f + static_cast<float>(rand()) / RAND_MAX * 500.0f; // Z entre -500 y 0 (mitad del suelo)

            createRandomSolid(Vector3(x, y, z), cameraTransform);
        }
    }

    Tipo getTipo() const { return Tipo::SOLIDO; }

    static float getApproximateSize() {
        return 5.0f;
    }
    void handleSolidCollision(PxRigidDynamic* solid) {
        solidManager.hit();
    }
    auto getSolids()
    {
        return solidManager.getSolids();
    }


private:
    PxPhysics* gPhysics;
    PxScene* gScene;
    SolidManager& solidManager; 
    ParticleSystem* particleS = new ParticleSystem();
    bool posteO;

    Tipo tipo = Tipo::SOLIDO;
};
