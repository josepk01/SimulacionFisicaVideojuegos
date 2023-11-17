#include <ctype.h>
#include <PxPhysicsAPI.h>
#include <vector>
#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include <iostream>
#include "Particle.h"
#include "ParticleSystem.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
#include "GravityForceGenerator.h"
#include "WindForceGenerator.h"
#include "VortexForceGenerator.h"
#include "ExplosionForceGenerator.h"

std::string display_text = "This is a test";
using namespace physx;

PxDefaultAllocator    gAllocator;
PxDefaultErrorCallback  gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;
PxMaterial* gMaterial = NULL;
PxPvd* gPvd = NULL;
PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;
double gameTime;

ParticleSystem* particleSystem = new ParticleSystem();


enum ParticleGeneratorType {
    DEFAULT_GENERATOR,
    GAUSSIAN_GENERATOR,
    UNIFORM_GENERATOR,
    FIREWORK_GENERATOR 
};


ParticleGeneratorType currentGeneratorType = DEFAULT_GENERATOR;


void shootProjectile(const PxTransform& camera) {
    Vector3 pos = Vector3(camera.p.x, camera.p.y, camera.p.z);
    PxVec3 direction = camera.q.getBasisVector2() * -1;
    float speed = 30.0f;
    Vector3 v = Vector3(direction.x * speed, direction.y * speed, direction.z * speed);
    Vector3 acceleration = Vector3(0, -9.81, 0);
    double damping = 0.99;
    double mass = 136.32;

    switch (currentGeneratorType) {
    case DEFAULT_GENERATOR:
        particleSystem->createParticle(pos, v, acceleration, damping, mass, false);
        break;
    case GAUSSIAN_GENERATOR:
        particleSystem->createParticleUsingGenerator("GaussianGenerator");
        break;
    case UNIFORM_GENERATOR:
        particleSystem->createParticleUsingGenerator("UniformGenerator");
        break;
    case FIREWORK_GENERATOR: // Añade esta sección
        particleSystem->createParticle(Vector3(0,20,0), v, acceleration, damping, mass, true);
        break;
    default:
        break;
    }
}

void initPhysics(bool interactive) {
    PX_UNUSED(interactive);
    gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);
    gPvd = PxCreatePvd(*gFoundation);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = contactReportFilterShader;
    sceneDesc.simulationEventCallback = &gContactReportCallback;
    gScene = gPhysics->createScene(sceneDesc);
    // Añade generadores de partículas
    particleSystem->addParticleGenerator(new GaussianParticleGenerator("GaussianGenerator", { -10, 20, 0 }, { 6, 6, 6 }, 1, 0.5));
    particleSystem->addParticleGenerator(new UniformParticleGenerator("UniformGenerator", { 10, 20, 0 }, { 6, 6, 6 }, 1, 0.5));

    GravityForceGenerator* gravityGen = new GravityForceGenerator(Vector3(0, -9.81f, 0));
    WindForceGenerator* windGen = new WindForceGenerator(Vector3(0.0f, 0.0f, 0.0f), 0 /*100*/); // Asumiendo un viento de 10 en el eje X
    // Crear el generador de torbellinos con un centro específico y una constante K
    VortexForceGenerator* vortexGen = new VortexForceGenerator(Vector3(0, 0, 0), 10000.0f); // Ejemplo de fuerza del torbellino
    // Crear el generador de explosiones con un centro específico y una constante K, R y tau
    ExplosionForceGenerator* explosionGen = new ExplosionForceGenerator(Vector3(0, 0, 0), 00000.0f, 100.0f, 10.0f);

    particleSystem->addForceGenerator(gravityGen);
    particleSystem->addForceGenerator(windGen);
    particleSystem->addForceGenerator(vortexGen);
    particleSystem->addForceGenerator(explosionGen);
    particleSystem->addExplosionGenerator(explosionGen);
}

void stepPhysics(bool interactive, double t) {
    gScene->simulate(t);
    gScene->fetchResults(true);
    particleSystem->updateParticles(t);    
    particleSystem->integrate(t);
    gameTime = t;

}

void cleanupPhysics(bool interactive) {
    delete particleSystem;
    PX_UNUSED(interactive);
    gScene->release();
    gDispatcher->release();
    gPhysics->release();
    PxPvdTransport* transport = gPvd->getTransport();
    gPvd->release();
    transport->release();
    gFoundation->release();
}

void keyPress(unsigned char key, const PxTransform& camera) {
    switch (toupper(key)) {
    case ' ':
        shootProjectile(camera);
        break;
    case '1':
        currentGeneratorType = DEFAULT_GENERATOR;
        std::cout << "Selected Default Particle Generator." << std::endl;
        break;
    case '2':
        currentGeneratorType = GAUSSIAN_GENERATOR;
        std::cout << "Selected Gaussian Particle Generator." << std::endl;
        break;
    case '3':
        currentGeneratorType = UNIFORM_GENERATOR;
        std::cout << "Selected Uniform Particle Generator." << std::endl;
        break;
    case '4': 
        currentGeneratorType = FIREWORK_GENERATOR;
        std::cout << "Selected Firework Particle Generator." << std::endl;
        break;
    case 'E': // Supongamos que 'E' es la tecla para detonar la explosión
        particleSystem->detonateExplosion(gameTime);
        break;
    default:
        break;
    }
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {
    PX_UNUSED(actor1);
    PX_UNUSED(actor2);
}

int main(int, const char* const*) {
#ifndef OFFLINE_EXECUTION
    extern void renderLoop();
    renderLoop();
#else
    static const PxU32 frameCount = 100;
    initPhysics(false);
    for (PxU32 i = 0; i < frameCount; i++)
        stepPhysics(false, 1.0 / 60.0); // Suponiendo un delta de tiempo de 1/60 por fotograma
    cleanupPhysics(false);
#endif

    return 0;
}
