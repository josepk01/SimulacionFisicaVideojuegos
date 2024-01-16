#pragma once
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
#include "SpringForceGenerator.h"
#include "BungeeForceGenerator.h"
#include "BuoyancyForceGenerator.h"
#include "SolidGenerator.h"
#include "GaussianSolidGen.h"
#include "SolidManager.h"
std::string display_text = "This is a test";
using namespace physx;

PxDefaultAllocator gAllocator;
PxDefaultErrorCallback gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;
PxMaterial* gMaterial = NULL;
PxPvd* gPvd = NULL;
PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;
double gameTime;

ParticleSystem* particleSystem = new ParticleSystem();
SolidManager* solidManager = new SolidManager(gScene);
enum ParticleGeneratorType {
    DEFAULT_GENERATOR,
    GAUSSIAN_GENERATOR,
    UNIFORM_GENERATOR,
    FIREWORK_GENERATOR
};
enum NamedForceGenerator {
    VORTEXFORCEGENERATOR,
    SPRINGFORCEGENERATOR,
    GRAVITYFORCEGENERATOR,
    EXPLOSIONFORCEGENERATOR,
    WINDFORCEGENERATOR,
    BUOYANCYFORCEGENERATOR
};
enum ParticleShotType {
    PISTOLA_GENERATOR,
    AK_GENERATOR,
    AWP_GENERATOR,
    CAL50_GENERATOR
};
ParticleShotType ShootType = PISTOLA_GENERATOR;
ParticleGeneratorType currentGeneratorType = DEFAULT_GENERATOR;
NamedForceGenerator currentForceGeneratorType = GRAVITYFORCEGENERATOR;
// Variables Globales
Vector3 anchorPoint = Vector3(0, 10, 0); // Punto de anclaje del muelle
float springConstant = 10.0f;            // Constante del muelle
float restLength = 5.0f;                 // Longitud de reposo del muelle
Particle* springParticle = nullptr;      // Partícula unida al muelle
float masa = 15.0f;
SpringForceGenerator* spring;
SolidGenerator* solid;
Particle* particleA = nullptr;  // Primera partícula
Particle* particleB = nullptr;  // Segunda partícula
Particle* particleC = nullptr;  // Segunda partícula
Particle* particleD = nullptr;  // Segunda partícula
Particle* particleE = nullptr;  // Segunda partícula
BungeeForceGenerator* bungee1 = nullptr; // Generador de fuerzas de goma elástica
BungeeForceGenerator* bungee2 = nullptr; // Generador de fuerzas de goma elástica
BungeeForceGenerator* bungee3 = nullptr; // Generador de fuerzas de goma elástica
BungeeForceGenerator* bungee4 = nullptr; // Generador de fuerzas de goma elástica
BungeeForceGenerator* bungee5 = nullptr; // Generador de fuerzas de goma elástica
GaussianSolidGenerator* gaussianGenerator=nullptr;
// Función auxiliar para activar/desactivar generadores de fuerza
void toggleForceGenerator(const std::string& name, NamedForceGenerator type) {
    if (currentForceGeneratorType == type) {
        particleSystem->deactivateForceGenerator(name);
        currentForceGeneratorType = GRAVITYFORCEGENERATOR; // Vuelve a la gravedad
    }
    else {
        particleSystem->activateForceGenerator(name);
        currentForceGeneratorType = type;
    }
}
void shootProjectile(const PxTransform& camera) {
    Vector3 pos = Vector3(camera.p.x, camera.p.y, camera.p.z);
    PxVec3 direction = camera.q.getBasisVector2() * -1;
    Vector3 acceleration = Vector3(0, -9.81, 0);
    double damping = 0.99;


    float speed;
    Vector3 v;
    double mass;

    switch (ShootType) {
    case PISTOLA_GENERATOR:
        speed = 100.0f;
        v = Vector3(direction.x * speed, direction.y * speed, direction.z * speed);
        mass = 28.5;
        particleSystem->createParticle(pos, v, acceleration, damping, mass, false);
        break;
    case AK_GENERATOR:
        speed = 188.0f;
        v = Vector3(direction.x * speed, direction.y * speed, direction.z * speed);
        mass = 30.1;
        particleSystem->createParticle(pos, v, acceleration, damping, mass, false);
        break;
    case AWP_GENERATOR:
        speed = 220.0f;
        v = Vector3(direction.x * speed, direction.y * speed, direction.z * speed);
        mass = 36.3;
        particleSystem->createParticle(pos, v, acceleration, damping, mass, false);
        break;
    case CAL50_GENERATOR:
        speed = 234.0f;
        v = Vector3(direction.x * speed, direction.y * speed, direction.z * speed);
        mass = 160.3;
        particleSystem->createParticle(pos, v, acceleration, damping, mass, false);
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

    // Crear y añadir generadores de fuerzas
    GravityForceGenerator* gravityGen = new GravityForceGenerator("Gravity", Vector3(0, -9.81f, 0));
    particleSystem->addForceGenerator(gravityGen);  // Gravedad siempre activa

    // Otros generadores de fuerzas (no activos inicialmente)
    WindForceGenerator* windGen = new WindForceGenerator("Wind", Vector3(0.0f, 0.0f, 0.0f), 0);
    particleSystem->addForceGenerator(windGen);

    VortexForceGenerator* vortexGen = new VortexForceGenerator("Vortex", Vector3(0, 0, 0), 10000.0f);
    particleSystem->addForceGenerator(vortexGen);

    ExplosionForceGenerator* explosionGen = new ExplosionForceGenerator("Explosion", Vector3(0, 0, 0), 10000.0f, 100.0f, 10.0f);
    particleSystem->addExplosionGenerator(explosionGen);

    SpringForceGenerator* springGen = new SpringForceGenerator("Spring", anchorPoint, springConstant, restLength);
    particleSystem->addForceGenerator(springGen);
    spring = springGen;

    // Crear una partícula y añadirla al sistema
    springParticle = new Particle(Vector3(0, 0, 15), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);
    particleSystem->addParticle(springParticle); // Añadir la partícula al sistema

    // Crear dos partículas
    particleA = new Particle(Vector3(0, 5, 0), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);
    particleB = new Particle(Vector3(0, 15, 0), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);

    // Añadir las partículas al sistema
    particleSystem->addParticle(particleA);
    particleSystem->addParticle(particleB);

    // Crear y añadir el generador de fuerzas de goma elástica
    bungee1 = new BungeeForceGenerator("bungee1", particleB, 20.0f, 5.0f);
    bungee2 = new BungeeForceGenerator("bungee2", particleA, 20.0f, 5.0f);
    particleSystem->addForceGenerator(bungee1);
    particleSystem->addForceGenerator(bungee2);

    //----opcional goma
    particleC = new Particle(Vector3(5, 0, 0), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);
    particleD = new Particle(Vector3(15, 0, 0), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);
    particleE = new Particle(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);
    particleSystem->addParticle(particleC);
    particleSystem->addParticle(particleD);
    particleSystem->addParticle(particleE);
    bungee3 = new BungeeForceGenerator("bungee3", particleC, 20.0f, 5.0f);
    bungee4 = new BungeeForceGenerator("bungee4", particleD, 20.0f, 5.0f);
    bungee5 = new BungeeForceGenerator("bungee5", particleE, 20.0f, 5.0f);
    particleSystem->addForceGenerator(bungee3);
    particleSystem->addForceGenerator(bungee4);
    particleSystem->addForceGenerator(bungee5);
    //----------------------------

    // Crear una partícula para el objeto flotante
    float objectVolume = 5.0f; // El volumen del objeto (ajustar según sea necesario)
    float objectMass = 10.0f; // La masa del objeto (ajustar según sea necesario)
    Particle* floatingObject = new Particle(Vector3(0, 10, 0), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, objectMass, objectVolume, false);

    // Añadir la partícula del objeto flotante al sistema
    particleSystem->addParticle(floatingObject);

    // Crear y añadir el generador de fuerzas de flotación
    float waterHeight = 0.0f; // La altura de la superficie del agua
    float liquidDensity = 1000.0f; // La densidad del agua
    float maxDepth = 20.0f;//profundidad de agua
    BuoyancyForceGenerator* buoyancyGen = new BuoyancyForceGenerator("buoyancy", maxDepth, objectVolume, waterHeight, liquidDensity);
    particleSystem->addForceGenerator(buoyancyGen);

    // Para el suelo PX
    sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);


    // Generar suelo
    PxRigidStatic* Suelo = gPhysics->createRigidStatic(PxTransform({ 0, 0, 0 }));

    PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 100));
    Suelo->attachShape(*shape);

    gScene->addActor(*Suelo);

    RenderItem* item;
    item = new RenderItem(shape, Suelo, { 0.8,0.8,0.8,1 });//despues de esto se pinta el agua -_-

    //// Añadir un actor dinámico

    //PxRigidDynamic* new_solid;

    //new_solid = gPhysics->createRigidDynamic(PxTransform({ -70, 200, -70 }));
    //new_solid->setLinearVelocity({ 0, 5, 0 });
    //new_solid->setAngularVelocity({ 0, 0, 0 });
    //PxShape* shape_ad = CreateShape(PxBoxGeometry(5, 5, 5));
    //new_solid->attachShape(*shape_ad);

    //PxRigidBodyExt::updateMassAndInertia(*new_solid, 0.15);
    //gScene->addActor(*new_solid);

    //// Pintar actor dinámico
    //RenderItem* dynamic_item;

    //dynamic_item = new RenderItem(shape_ad, new_solid, {0.8, 0.8, 0.8,1 });

    solid = new SolidGenerator(gPhysics, gScene, *solidManager, particleSystem);

    gaussianGenerator = new GaussianSolidGenerator(gPhysics, gScene, *solidManager, particleSystem, Vector3(-70, 200, -70), 5.0, 5.0, 25.0);

}


void stepPhysics(bool interactive, double t) {
    gScene->simulate(t);
    gScene->fetchResults(true);
    particleSystem->updateParticles(t);
    particleSystem->integrate(t);
    solidManager->integrate(t); // Aquí se actualizan los sólidos
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
        ShootType = PISTOLA_GENERATOR;
        std::cout << "Has sacado la pistola" << std::endl;
        break;
    case '2':
        ShootType = AK_GENERATOR;
        std::cout << "Has sacado el AK-47" << std::endl;
        break;
    case '3':
        ShootType = AWP_GENERATOR;
        std::cout << "Has sacado el AWP" << std::endl;
        break;
    case '4':
        ShootType = CAL50_GENERATOR;
        std::cout << "Has sacado el Cal.50" << std::endl;
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