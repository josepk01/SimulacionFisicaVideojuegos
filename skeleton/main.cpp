#include <ctype.h> // Incluye la biblioteca ctype.h

#include <PxPhysicsAPI.h> // Incluye la biblioteca de la API de Physics (PhysX)

#include <vector> // Incluye la biblioteca vector

#include "core.hpp" // Incluye el archivo core.hpp
#include "RenderUtils.hpp" // Incluye el archivo RenderUtils.hpp
#include "callbacks.hpp" // Incluye el archivo callbacks.hpp

#include <iostream> // Incluye la biblioteca de entrada/salida estándar
#include "Particle.h"

std::string display_text = "This is a test"; // Declara una cadena de texto llamada "display_text" con un valor inicial

using namespace physx; // Usa el espacio de nombres physx

PxDefaultAllocator    gAllocator; // Declara un objeto de tipo PxDefaultAllocator llamado gAllocator
PxDefaultErrorCallback  gErrorCallback; // Declara un objeto de tipo PxDefaultErrorCallback llamado gErrorCallback

PxFoundation* gFoundation = NULL; // Declara un puntero a PxFoundation llamado gFoundation e inicialízalo como NULL
PxPhysics* gPhysics = NULL; // Declara un puntero a PxPhysics llamado gPhysics e inicialízalo como NULL

PxMaterial* gMaterial = NULL; // Declara un puntero a PxMaterial llamado gMaterial e inicialízalo como NULL

PxPvd* gPvd = NULL; // Declara un puntero a PxPvd llamado gPvd e inicialízalo como NULL

PxDefaultCpuDispatcher* gDispatcher = NULL; // Declara un puntero a PxDefaultCpuDispatcher llamado gDispatcher e inicialízalo como NULL
PxScene* gScene = NULL; // Declara un puntero a PxScene llamado gScene e inicialízalo como NULL
ContactReportCallback gContactReportCallback; // Declara un objeto de tipo ContactReportCallback llamado gContactReportCallback

Particle* particle;
std::vector<Particle*> projectiles; // Vector para almacenar múltiples partículas (proyectiles)
int selectedProjectileType = 0; // Por defecto, seleccionamos el primer tipo de proyectil


void shootProjectile(const PxTransform& camera) {
    Vector3 pos = Vector3(camera.p.x, camera.p.y, camera.p.z);
    PxVec3 direction = camera.q.getBasisVector2() * -1;
    float speed = 30.0f;

    Vector3 v = Vector3(direction.x * speed, direction.y * speed, direction.z * speed);
    Vector3 acceleration = Vector3(0, -9.81, 0);
    double damping = 0.99;

    double mass = 136.32; // Masa calculada
    Particle* newProjectile = new Particle(pos, v, acceleration, damping, mass);
    projectiles.push_back(newProjectile);
}



// Inicializa el motor de física
void initPhysics(bool interactive)
{
    PX_UNUSED(interactive); // No se utiliza la variable 'interactive'

    gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback); // Crea una fundación de PhysX

    gPvd = PxCreatePvd(*gFoundation); // Crea una instancia de PxPvd (Visualización de Depuración de PhysX)
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10); // Crea un transporte para la visualización de depuración
    gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL); // Conecta la instancia de PxPvd al transporte

    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd); // Crea una instancia de PxPhysics

    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f); // Crea un material de física

    // Configuración de la escena de física
    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f); // Establece la gravedad
    gDispatcher = PxDefaultCpuDispatcherCreate(2); // Crea un despachador para la CPU
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = contactReportFilterShader; // Establece una función para el filtrado de colisiones
    sceneDesc.simulationEventCallback = &gContactReportCallback; // Establece un callback para eventos de simulación
    gScene = gPhysics->createScene(sceneDesc); // Crea una escena de física

    Particle* newProjectile = new Particle(Vector3(-10, 20, 0), Vector3(20, 0, 0), Vector3(0, -9.81, 0), 0.99, 136.32);
    projectiles.push_back(newProjectile);
}

// Función para realizar un paso de física
void stepPhysics(bool interactive, double t) {
    gScene->simulate(t);
    gScene->fetchResults(true);

    for (Particle* proj : projectiles) {
        proj->integrate(t);
    }
}

// Función para limpiar los datos
void cleanupPhysics(bool interactive)
{
    delete particle;

    for (Particle* proj : projectiles) {
        delete proj;
    }
    projectiles.clear();

    PX_UNUSED(interactive); // No se utiliza la variable 'interactive'

    gScene->release(); // Libera la escena
    gDispatcher->release(); // Libera el despachador
    gPhysics->release(); // Libera la instancia de PxPhysics
    PxPvdTransport* transport = gPvd->getTransport(); // Obtiene el transporte de PxPvd
    gPvd->release(); // Libera la instancia de PxPvd
    transport->release(); // Libera el transporte
    gFoundation->release(); // Libera la fundación


}

// Función llamada cuando se presiona una tecla
void keyPress(unsigned char key, const PxTransform& camera) {
    switch (toupper(key)) {
    case ' ':
        shootProjectile(camera); // Disparamos un proyectil cuando se pulsa el espacio
        break;
    case '1': 
        selectedProjectileType = 0;
        break;
    default:
        break;
    }
}


//Funcion de manejo de colisiones (ahora vacio)
void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
    PX_UNUSED(actor1);
    PX_UNUSED(actor2);
}

int main(int, const char* const*)
{
#ifndef OFFLINE_EXECUTION
    extern void renderLoop(); // Declaración de una función externa llamada renderLoop
    renderLoop(); // Llama a la función renderLoop
#else
    static const PxU32 frameCount = 100; // Número de fotogramas para la ejecución fuera de línea
    initPhysics(false); // Inicializa la física en modo fuera de línea
    for (PxU32 i = 0; i < frameCount; i++)
        stepPhysics(false); // Realiza pasos de física en modo fuera de línea
    cleanupPhysics(false); // Limpia los datos en modo fuera de línea
#endif

    return 0; // Devuelve 0 como código de salida
}
