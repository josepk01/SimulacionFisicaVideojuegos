#pragma once
#include <ctype.h>
#include <PxPhysicsAPI.h>
#include <Time.h>
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
#include "CollisionManager.h"
std::string display_text = "+";
std::string display_text_win = "+";
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
CollisionManager* colison = new CollisionManager();
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
    BUOYANCYFORCEGENERATOR,
    NINGUNO
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

double lastShotTime = 0; // Tiempo desde el último disparo
double fireRatePistola = 0.005; // 0.5 segundos entre disparos para la pistola
double fireRateAK = 0.001; // 0.1 segundos entre disparos para el AK-47
double fireRateAWP = 0.015; // 1.5 segundos entre disparos para el AWP
double fireRateCal50 = 0.020; // 2 segundos entre disparos para el Cal.50

int destructionCount = 0;

Vector3 v;


std::string fuerza_actual;
// Función auxiliar para activar/desactivar generadores de fuerza
void toggleForceGenerator(const std::string& name, NamedForceGenerator type) {
    if (currentForceGeneratorType == type) {
        particleSystem->deactivateForceGenerator(name);
        currentForceGeneratorType = NINGUNO;
        fuerza_actual = "Null";
    }
    else {
        particleSystem->deactivateForceGenerator(fuerza_actual);
        fuerza_actual = name;
        particleSystem->activateForceGenerator(name);
        currentForceGeneratorType = type;
    }
}

void shootProjectile(const PxTransform& camera) {

    double currentTime = gameTime;
    double timeSinceLastShot = currentTime - lastShotTime;

    double fireRate;
    switch (ShootType) {
    case PISTOLA_GENERATOR:
        fireRate = fireRatePistola;
        break;
    case AK_GENERATOR:
        fireRate = fireRateAK;
        break;
    case AWP_GENERATOR:
        fireRate = fireRateAWP;
        break;
    case CAL50_GENERATOR:
        fireRate = fireRateCal50;
        break;
    default:
        fireRate = 1.0; // Cadencia por defecto
        break;
    }
    //if (timeSinceLastShot >= fireRate) { esto era un intento de tener cadencia pero parece que el tiempo no funciona bien 
    //aun asi esta es la idea q se tenia


        Vector3 pos = Vector3(camera.p.x, camera.p.y, camera.p.z);
        PxVec3 direction = camera.q.getBasisVector2() * -1;
        Vector3 acceleration = Vector3(0, -9.81, 0);
        double damping = 0.99;


        float speed = 30;
        v = Vector3(direction.x * speed, direction.y * speed, direction.z * speed);;
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
        lastShotTime = currentTime;
   // }
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
    particleSystem->activateForceGenerator("Gravity");

    // Otros generadores de fuerzas (no activos inicialmente)
    WindForceGenerator* windGen = new WindForceGenerator("Wind", Vector3(0.0f, 0.0f, 0.0f), 0);
    particleSystem->addForceGenerator(windGen);
    // Crear generadores de viento
    WindForceGenerator* variableWindGen = new WindForceGenerator("VariableWind", Vector3(0.0f, 0.0f, 0.0f), 0);
    particleSystem->addForceGenerator(variableWindGen);

    WindForceGenerator* constantWindGen1 = new WindForceGenerator("ConstantWind1", Vector3(10.0f, 0.0f, 0.0f), 50.0f);
    particleSystem->addForceGenerator(constantWindGen1);

    WindForceGenerator* constantWindGen2 = new WindForceGenerator("ConstantWind2", Vector3(-15.0f, 0.0f, 0.0f), 50.0f);
    particleSystem->addForceGenerator(constantWindGen2);

    WindForceGenerator* constantWindGen3 = new WindForceGenerator("ConstantWind3", Vector3(0.0f, 15.0f, 10.0f), 50.0f);
    particleSystem->addForceGenerator(constantWindGen3);

    VortexForceGenerator* vortexGen = new VortexForceGenerator("Vortex", Vector3(0, 0, 0), 10000.0f);
    particleSystem->addForceGenerator(vortexGen);

    ExplosionForceGenerator* explosionGen = new ExplosionForceGenerator("Explosion", Vector3(0, 0, 0), 10000.0f, 100.0f, 10.0f);
    particleSystem->addExplosionGenerator(explosionGen);

    // Para el suelo PX
    sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
    
    // Configuración del suelo (en initPhysics)
    PxRigidStatic* Suelo = gPhysics->createRigidStatic(PxTransform(PxVec3(0, 0, 0)));
    PxShape* shape = CreateShape(PxBoxGeometry(100, 0.1, 1000)); // Más largo en una dirección
    Suelo->attachShape(*shape);
    gScene->addActor(*Suelo);

    // Configuración del color (marrón/gris)
    Vector4 sueloColor = Vector4(0.5, 0.5, 0.5, 1); // Gris, ajustar para marrón si es necesario
    RenderItem* item = new RenderItem(shape, Suelo, sueloColor);

    //RenderItem* item;
    //item = new RenderItem(shape, Suelo, { 0.8,0.8,0.8,1 });//despues de esto se pinta el agua -_-


    solid = new SolidGenerator(gPhysics, gScene, *solidManager, particleSystem);

    gaussianGenerator = new GaussianSolidGenerator(gPhysics, gScene, *solidManager, particleSystem, Vector3(-70, 200, -70), 5.0, 5.0, 25.0);


    // Crear partículas y aplicar la fuerza del resorte
    for (int i = 0; i < 20; ++i) {
        Vector3 anchorPointSpring = Vector3(-50, 10, -900 + (i * 60));
        float springConstantSpring = 1.0f;
        float restLengthSpring = 10.0f;

        Vector3 particlePosition = Vector3(-50, 10, -900 + (i * 60));
        Particle* particle = new Particle(particlePosition, Vector3(0, 0, 0), Vector3(0, -9.81, 0), 0.01, 1.0, 9999999, false);
        particleSystem->addParticle(particle);

        SpringForceGenerator* springGen = new SpringForceGenerator("Spring" + std::to_string(i), particle, anchorPointSpring, springConstantSpring, restLengthSpring);
        particleSystem->addForceGenerator(springGen);
        particleSystem->activateForceGenerator(springGen->getName());

        // Crear el poste
        solid->createPost(Vector3(-50, 5, -900 + (i * 60)), Vector3(1, 10, 1), Vector4(0.5, 0.3, 0.2, 1)); // Ajusta los tamaños y colores según sea necesario
    }


    toggleForceGenerator("ConstantWind1", CONSTANT_WINDFORCEGENERATOR_1);


}

void activateFireworks() {
    Vector3 positions[3] = { Vector3(40, 0, -980), Vector3(0, 0, -980), Vector3(-40, 0, -980) };
    for (const auto& pos : positions) {
        // Asumiendo que tienes una función para crear y activar un fuego artificial
        // Esta función debe ser implementada según la lógica de tus fuegos artificiales
        particleSystem->createParticle(pos, v, Vector3(0, -9.81, 0), 0.99, 1, true);
    }
}
void stepPhysics(bool interactive, double t) {
    gScene->simulate(t);
    gScene->fetchResults(true);
    particleSystem->updateParticles(t);
    particleSystem->integrate(t);
    solidManager->integrate(t); // Aquí se actualizan los sólidos
    if (colison->checkCollisions(particleSystem, solidManager))
    {
        destructionCount++;


        // Verifica si el contador superó un múltiplo de 10
        if (destructionCount % 10 == 0) {
            // Activa fuegos artificiales
            activateFireworks();
        }
    }
    // Actualiza el texto del contador
    display_text_win = "Has destruido " + std::to_string(destructionCount)+" objetos";
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
    case 'Z': // Activar/Desactivar generador de viento
        toggleForceGenerator("ConstantWind1", CONSTANT_WINDFORCEGENERATOR_1);
        break;
    case 'X': // Activar/Desactivar generador de viento
        toggleForceGenerator("ConstantWind2", CONSTANT_WINDFORCEGENERATOR_2);
        break;
    case 'C': // Activar/Desactivar generador de viento
        toggleForceGenerator("ConstantWind3", CONSTANT_WINDFORCEGENERATOR_3);
        break;
    case 'V': // Activar/Desactivar generador de viento
        toggleForceGenerator("VariableWind", VARIABLE_WINDFORCEGENERATOR);
        break;   
    case '0': // Activar/Desactivar generador de viento
        solid->generateRandomSolids(10, camera);
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