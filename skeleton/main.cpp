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
    WINDFORCEGENERATOR
};

ParticleGeneratorType currentGeneratorType = DEFAULT_GENERATOR;
NamedForceGenerator currentForceGeneratorType = GRAVITYFORCEGENERATOR;
// Variables Globales
Vector3 anchorPoint = Vector3(0, 10, 0); // Punto de anclaje del muelle
float springConstant = 10.0f;            // Constante del muelle
float restLength = 5.0f;                 // Longitud de reposo del muelle
Particle* springParticle = nullptr;      // Partícula unida al muelle
SpringForceGenerator* spring;
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
    case FIREWORK_GENERATOR:
        particleSystem->createParticle(Vector3(0, 20, 0), v, acceleration, damping, mass, true);
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
    particleSystem->addForceGenerator(explosionGen);

    SpringForceGenerator* springGen = new SpringForceGenerator("Spring", anchorPoint, springConstant, restLength);
    particleSystem->addForceGenerator(springGen);
    spring = springGen;

    //// Crear una partícula y añadirla al sistema
    //springParticle = new Particle(Vector3(0, 0, 15), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);
    //particleSystem->addParticle(springParticle); // Añadir la partícula al sistema

    //// Crear dos partículas
    //particleA = new Particle(Vector3(0, 5, 0), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);
    //particleB = new Particle(Vector3(0, 15, 0), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);

    //// Añadir las partículas al sistema
    //particleSystem->addParticle(particleA);
    //particleSystem->addParticle(particleB);

    //// Crear y añadir el generador de fuerzas de goma elástica
    //bungee1 = new BungeeForceGenerator("bungee1",particleB, 20.0f, 5.0f);
    //bungee2 = new BungeeForceGenerator("bungee2", particleA, 20.0f, 5.0f);
    //particleSystem->addForceGenerator(bungee1);    
    //particleSystem->addForceGenerator(bungee2);

    ////----opcional goma
    //particleC = new Particle(Vector3(5, 0, 0), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);
    //particleD = new Particle(Vector3(15, 0, 0), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);
    //particleE = new Particle(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);
    //particleSystem->addParticle(particleC);
    //particleSystem->addParticle(particleD);
    //particleSystem->addParticle(particleE);
    //bungee3 = new BungeeForceGenerator("bungee3", particleC, 20.0f, 5.0f);
    //bungee4 = new BungeeForceGenerator("bungee4", particleD, 20.0f, 5.0f);
    //bungee5 = new BungeeForceGenerator("bungee5", particleE, 20.0f, 5.0f);
    //particleSystem->addForceGenerator(bungee3);
    //particleSystem->addForceGenerator(bungee4);
    //particleSystem->addForceGenerator(bungee5);
    //----------------------------
    // Esta es solo una representación visual y no necesita ser parte del sistema de partículas
    WaterSurface* waterSurface = new WaterSurface(Vector3(0, 0, 0), Vector3(100, 0.1, 100)); // Un cubo plano grande

    // Crear una partícula para el objeto flotante
    float objectVolume = 5.0f; // El volumen del objeto (ajustar según sea necesario)
    float objectMass = 10.0f; // La masa del objeto (ajustar según sea necesario)
    Particle* floatingObject = new Particle(Vector3(0, 10, 0), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, objectMass, objectVolume, false);

    // Añadir la partícula del objeto flotante al sistema
    particleSystem->addParticle(floatingObject);

    // Crear y añadir el generador de fuerzas de flotación
    float waterHeight = 0.0f; // La altura de la superficie del agua
    float liquidDensity = 1000.0f; // La densidad del agua
    float maxDepth = 100.0f;//profundidad de agua
    BuoyancyForceGenerator* buoyancyGen = new BuoyancyForceGenerator("buoyancy", maxDepth, objectVolume,waterHeight, liquidDensity);
    particleSystem->addForceGenerator(buoyancyGen);

    // Asociar la fuerza de flotación con la partícula del objeto flotante
    buoyancyGen->addParticle(floatingObject);
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
    case 'E': // Suponiendo que 'E' es la tecla para detonar la explosión
        particleSystem->detonateExplosion(gameTime);
        break;
    case 'G': // Gravedad siempre activa, no hace nada al presionar 'G'
        break;
    case 'W': // Activar/Desactivar generador de viento
        toggleForceGenerator("Wind", WINDFORCEGENERATOR);
        break;
    case 'V': // Activar/Desactivar generador de vórtice
        toggleForceGenerator("Vortex", VORTEXFORCEGENERATOR);
        break;
    case 'M': // Activar/Desactivar generador de muelle
        toggleForceGenerator("Spring", SPRINGFORCEGENERATOR);
        break;
    //case 'F':  // Aplicar fuerza a la partícula del muelle
    //    if (springParticle) {
    //        particleSystem->applyForceToParticle(springParticle, Vector3(10, 0, 0)); // Aplica una fuerza en dirección X
    //    }
    //    break;
    case '+': // Aumentar la constante del muelle
        springConstant += 10;
        spring->setSpringConstant(springConstant);
        break;
    case '-': // Disminuir la constante del muelle
        if (springConstant > 10) {
            springConstant -= 10;
            spring->setSpringConstant(springConstant);
        }
        break;
        // ... otros casos para otros generadores de fuerzas ...
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