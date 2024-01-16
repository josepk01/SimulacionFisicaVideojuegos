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
std::string display_text = "+";
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
    VARIABLE_WINDFORCEGENERATOR,
    CONSTANT_WINDFORCEGENERATOR_1,
    CONSTANT_WINDFORCEGENERATOR_2,
    CONSTANT_WINDFORCEGENERATOR_3,
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
Particle* springParticle = nullptr;      // Part�cula unida al muelle
float masa = 15.0f;
SpringForceGenerator* spring;
SolidGenerator* solid;
Particle* particleA = nullptr;  // Primera part�cula
Particle* particleB = nullptr;  // Segunda part�cula
Particle* particleC = nullptr;  // Segunda part�cula
Particle* particleD = nullptr;  // Segunda part�cula
Particle* particleE = nullptr;  // Segunda part�cula
BungeeForceGenerator* bungee1 = nullptr; // Generador de fuerzas de goma el�stica
BungeeForceGenerator* bungee2 = nullptr; // Generador de fuerzas de goma el�stica
BungeeForceGenerator* bungee3 = nullptr; // Generador de fuerzas de goma el�stica
BungeeForceGenerator* bungee4 = nullptr; // Generador de fuerzas de goma el�stica
BungeeForceGenerator* bungee5 = nullptr; // Generador de fuerzas de goma el�stica
GaussianSolidGenerator* gaussianGenerator=nullptr;
// Funci�n auxiliar para activar/desactivar generadores de fuerza
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

    //switch (currentGeneratorType) {
    //case DEFAULT_GENERATOR:
    //    particleSystem->createParticle(pos, v, acceleration, damping, mass, false);
    //    break;
    //case GAUSSIAN_GENERATOR:
    //    particleSystem->createParticleUsingGenerator("GaussianGenerator");
    //    break;
    //case UNIFORM_GENERATOR:
    //    particleSystem->createParticleUsingGenerator("UniformGenerator");
    //    break;
    //case FIREWORK_GENERATOR:
    //    particleSystem->createParticle(Vector3(0, 20, 0), v, acceleration, damping, mass, true);
    //    break;
    //default:
    //    break;
    //}

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

    // A�ade generadores de part�culas
    particleSystem->addParticleGenerator(new GaussianParticleGenerator("GaussianGenerator", { -10, 20, 0 }, { 6, 6, 6 }, 1, 0.5));
    particleSystem->addParticleGenerator(new UniformParticleGenerator("UniformGenerator", { 10, 20, 0 }, { 6, 6, 6 }, 1, 0.5));

    // Crear y a�adir generadores de fuerzas
    GravityForceGenerator* gravityGen = new GravityForceGenerator("Gravity", Vector3(0, -9.81f, 0));
    particleSystem->addForceGenerator(gravityGen);  // Gravedad siempre activa

    // Otros generadores de fuerzas (no activos inicialmente)
    WindForceGenerator* windGen = new WindForceGenerator("Wind", Vector3(0.0f, 0.0f, 0.0f), 0);
    particleSystem->addForceGenerator(windGen);
    // Crear generadores de viento
    WindForceGenerator* variableWindGen = new WindForceGenerator("VariableWind", Vector3(0.0f, 0.0f, 0.0f), 0);
    particleSystem->addForceGenerator(variableWindGen);

    WindForceGenerator* constantWindGen1 = new WindForceGenerator("ConstantWind1", Vector3(50.0f, 50.0f, 50.0f), 50.0f);
    particleSystem->addForceGenerator(constantWindGen1);

    WindForceGenerator* constantWindGen2 = new WindForceGenerator("ConstantWind2", Vector3(-5.0f, 0.0f, 0.0f), 0);
    particleSystem->addForceGenerator(constantWindGen2);

    WindForceGenerator* constantWindGen3 = new WindForceGenerator("ConstantWind3", Vector3(0.0f, 5.0f, 0.0f), 0);
    particleSystem->addForceGenerator(constantWindGen3);

    VortexForceGenerator* vortexGen = new VortexForceGenerator("Vortex", Vector3(0, 0, 0), 10000.0f);
    particleSystem->addForceGenerator(vortexGen);

    ExplosionForceGenerator* explosionGen = new ExplosionForceGenerator("Explosion", Vector3(0, 0, 0), 10000.0f, 100.0f, 10.0f);
    particleSystem->addExplosionGenerator(explosionGen);

    SpringForceGenerator* springGen = new SpringForceGenerator("Spring", anchorPoint, springConstant, restLength);
    particleSystem->addForceGenerator(springGen);
    spring = springGen;

    //// Crear una part�cula y a�adirla al sistema
    //springParticle = new Particle(Vector3(0, 0, 15), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);
    //particleSystem->addParticle(springParticle); // A�adir la part�cula al sistema

    //// Crear dos part�culas
    //particleA = new Particle(Vector3(0, 5, 0), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);
    //particleB = new Particle(Vector3(0, 15, 0), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, 1.0, 1000, false);

    //// A�adir las part�culas al sistema
    //particleSystem->addParticle(particleA);
    //particleSystem->addParticle(particleB);

    //// Crear y a�adir el generador de fuerzas de goma el�stica
    //bungee1 = new BungeeForceGenerator("bungee1", particleB, 20.0f, 5.0f);
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
    ////----------------------------

    //// Crear una part�cula para el objeto flotante
    //float objectVolume = 5.0f; // El volumen del objeto (ajustar seg�n sea necesario)
    //float objectMass = 10.0f; // La masa del objeto (ajustar seg�n sea necesario)
    //Particle* floatingObject = new Particle(Vector3(0, 10, 0), Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.99, objectMass, objectVolume, false);

    //// A�adir la part�cula del objeto flotante al sistema
    //particleSystem->addParticle(floatingObject);

    //// Crear y a�adir el generador de fuerzas de flotaci�n
    //float waterHeight = 0.0f; // La altura de la superficie del agua
    //float liquidDensity = 1000.0f; // La densidad del agua
    //float maxDepth = 20.0f;//profundidad de agua
    //BuoyancyForceGenerator* buoyancyGen = new BuoyancyForceGenerator("buoyancy", maxDepth, objectVolume, waterHeight, liquidDensity);
    //particleSystem->addForceGenerator(buoyancyGen);

    // Para el suelo PX
    sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
    
    // Configuraci�n del suelo (en initPhysics)
    PxRigidStatic* Suelo = gPhysics->createRigidStatic(PxTransform(PxVec3(0, 0, 0)));
    PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 1000)); // M�s largo en una direcci�n
    Suelo->attachShape(*shape);
    gScene->addActor(*Suelo);

    // Configuraci�n del color (marr�n/gris)
    Vector4 sueloColor = Vector4(0.5, 0.5, 0.5, 1); // Gris, ajustar para marr�n si es necesario
    RenderItem* item = new RenderItem(shape, Suelo, sueloColor);

    //RenderItem* item;
    //item = new RenderItem(shape, Suelo, { 0.8,0.8,0.8,1 });//despues de esto se pinta el agua -_-


    solid = new SolidGenerator(gPhysics, gScene, *solidManager, particleSystem);

    gaussianGenerator = new GaussianSolidGenerator(gPhysics, gScene, *solidManager, particleSystem, Vector3(-70, 200, -70), 5.0, 5.0, 25.0);





    // Crear part�culas y aplicar la fuerza del resorte
    for (int i = 0; i < 10; ++i) {
        // Puntos de anclaje y propiedades del resorte
        Vector3 anchorPointSpring = Vector3(-20, 10, -1000 + (i * 20));
        float springConstantSpring = 10.0f;
        float restLengthSpring = 100.0f;

        // Crear part�cula
        Vector3 particlePosition = Vector3(-20, 10, -1000 + (i * 20));
        Particle* particle = new Particle(particlePosition, Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.01, 1.0, 9999999, false);
        particleSystem->addParticle(particle);

        // Crear generador de fuerzas de resorte y conectarlo a la part�cula
        std::string springName = "Spring" + std::to_string(i);
        SpringForceGenerator* springGen = new SpringForceGenerator(springName, anchorPointSpring, springConstantSpring, restLengthSpring);

        //springGen->addParticle(particle); // Aseg�rate de que el m�todo addParticle exista y funcione correctamente
        particleSystem->addForceGenerator(springGen);
        particleSystem->activateForceGenerator(springName);
    }

    //particleSystem->activateForceGenerator("Gravity");
    particleSystem->activateForceGenerator("ConstantWind1");


}

void stepPhysics(bool interactive, double t) {
    gScene->simulate(t);
    gScene->fetchResults(true);
    particleSystem->updateParticles(t);
    particleSystem->integrate(t);
    solidManager->integrate(t); // Aqu� se actualizan los s�lidos
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

    //case ' ':
    //    shootProjectile(camera);
    //    break;
    //case '1':
    //    currentGeneratorType = DEFAULT_GENERATOR;
    //    std::cout << "Selected Default Particle Generator." << std::endl;
    //    break;
    //case '2':
    //    currentGeneratorType = GAUSSIAN_GENERATOR;
    //    std::cout << "Selected Gaussian Particle Generator." << std::endl;
    //    break;
    //case '3':
    //    currentGeneratorType = UNIFORM_GENERATOR;
    //    std::cout << "Selected Uniform Particle Generator." << std::endl;
    //    break;
    //case '4':
    //    currentGeneratorType = FIREWORK_GENERATOR;
    //    std::cout << "Selected Firework Particle Generator." << std::endl;
    //    break;
    //case 'E': // Suponiendo que 'E' es la tecla para detonar la explosi�n
    //    particleSystem->detonateExplosion(gameTime);
    //    break;
    case 'Z': // Activar/Desactivar generador de viento
        toggleForceGenerator("ConstantWind1", WINDFORCEGENERATOR);
        break;
    case 'X': // Activar/Desactivar generador de viento
        toggleForceGenerator("ConstantWind2", WINDFORCEGENERATOR);
        break;
    case 'C': // Activar/Desactivar generador de viento
        toggleForceGenerator("ConstantWind3", WINDFORCEGENERATOR);
        break;
    case 'V': // Activar/Desactivar generador de viento
        toggleForceGenerator("VariableWind", WINDFORCEGENERATOR);
        break;
    //case 'V': // Activar/Desactivar generador de v�rtice
    //    toggleForceGenerator("Vortex", VORTEXFORCEGENERATOR);
    //    break;
    //case 'M': // Activar/Desactivar generador de muelle
    //    toggleForceGenerator("Spring", SPRINGFORCEGENERATOR);
    //    break;
    //case '0': // Activar/Desactivar generador de muelle
    //    toggleForceGenerator("buoyancy", BUOYANCYFORCEGENERATOR);
    //    break;
    //case '+': // Aumentar la constante del muelle
    //    springConstant += 10;
    //    spring->setSpringConstant(springConstant);
    //    break;
    //case '-': // Disminuir la constante del muelle
    //    if (springConstant > 10) {
    //        springConstant -= 10;
    //        spring->setSpringConstant(springConstant);
    //    }
    //    break;
    //case 'O': {
    //    Vector3 position(-70, 200, -70);
    //    solid = new SolidGenerator(gPhysics, gScene, *solidManager,particleSystem);
    //    solid->createSolid(position, masa);
    //    break;
    //}
    //case 'L': {
    //    Vector3 position(-70, 200, -70);
    //    solid = new SolidGenerator(gPhysics, gScene, *solidManager,particleSystem);
    //    solid->createSolid(position, masa + 10);
    //    break;
    //}
    //case 'K': {
    //    Vector3 position(-70, 200, -70);
    //    solid = new SolidGenerator(gPhysics, gScene, *solidManager,particleSystem);
    //    solid->createSolid(position, fmax(masa - 10, 0.01f));
    //    break;
    //}
    //case 'Z': {
    //    gaussianGenerator->createSolid();
    //    break;
    //}
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