#include <ctype.h> // Incluye la biblioteca ctype.h

#include <PxPhysicsAPI.h> // Incluye la biblioteca de la API de Physics (PhysX)

#include <vector> // Incluye la biblioteca vector

#include "core.hpp" // Incluye el archivo core.hpp
#include "RenderUtils.hpp" // Incluye el archivo RenderUtils.hpp
#include "callbacks.hpp" // Incluye el archivo callbacks.hpp

#include <iostream> // Incluye la biblioteca de entrada/salida est�ndar

std::string display_text = "This is a test"; // Declara una cadena de texto llamada "display_text" con un valor inicial

using namespace physx; // Usa el espacio de nombres physx

PxDefaultAllocator    gAllocator; // Declara un objeto de tipo PxDefaultAllocator llamado gAllocator
PxDefaultErrorCallback  gErrorCallback; // Declara un objeto de tipo PxDefaultErrorCallback llamado gErrorCallback

PxFoundation* gFoundation = NULL; // Declara un puntero a PxFoundation llamado gFoundation e inicial�zalo como NULL
PxPhysics* gPhysics = NULL; // Declara un puntero a PxPhysics llamado gPhysics e inicial�zalo como NULL

PxMaterial* gMaterial = NULL; // Declara un puntero a PxMaterial llamado gMaterial e inicial�zalo como NULL

PxPvd* gPvd = NULL; // Declara un puntero a PxPvd llamado gPvd e inicial�zalo como NULL

PxDefaultCpuDispatcher* gDispatcher = NULL; // Declara un puntero a PxDefaultCpuDispatcher llamado gDispatcher e inicial�zalo como NULL
PxScene* gScene = NULL; // Declara un puntero a PxScene llamado gScene e inicial�zalo como NULL
ContactReportCallback gContactReportCallback; // Declara un objeto de tipo ContactReportCallback llamado gContactReportCallback

// Inicializa el motor de f�sica
void initPhysics(bool interactive)
{
    PX_UNUSED(interactive); // No se utiliza la variable 'interactive'

    gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback); // Crea una fundaci�n de PhysX

    gPvd = PxCreatePvd(*gFoundation); // Crea una instancia de PxPvd (Visualizaci�n de Depuraci�n de PhysX)
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10); // Crea un transporte para la visualizaci�n de depuraci�n
    gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL); // Conecta la instancia de PxPvd al transporte

    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd); // Crea una instancia de PxPhysics

    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f); // Crea un material de f�sica

    // Configuraci�n de la escena de f�sica
    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f); // Establece la gravedad
    gDispatcher = PxDefaultCpuDispatcherCreate(2); // Crea un despachador para la CPU
    sceneDesc.cpuDispatcher = gDispatcher;
    sceneDesc.filterShader = contactReportFilterShader; // Establece una funci�n para el filtrado de colisiones
    sceneDesc.simulationEventCallback = &gContactReportCallback; // Establece un callback para eventos de simulaci�n
    gScene = gPhysics->createScene(sceneDesc); // Crea una escena de f�sica
}

// Funci�n para realizar un paso de f�sica
void stepPhysics(bool interactive, double t)
{
    PX_UNUSED(interactive); // No se utiliza la variable 'interactive'

    gScene->simulate(t); // Simula la escena durante un tiempo 't'
    gScene->fetchResults(true); // Recupera los resultados de la simulaci�n
}

// Funci�n para limpiar los datos
void cleanupPhysics(bool interactive)
{
    PX_UNUSED(interactive); // No se utiliza la variable 'interactive'

    gScene->release(); // Libera la escena
    gDispatcher->release(); // Libera el despachador
    gPhysics->release(); // Libera la instancia de PxPhysics
    PxPvdTransport* transport = gPvd->getTransport(); // Obtiene el transporte de PxPvd
    gPvd->release(); // Libera la instancia de PxPvd
    transport->release(); // Libera el transporte
    gFoundation->release(); // Libera la fundaci�n
}

// Funci�n llamada cuando se presiona una tecla
void keyPress(unsigned char key, const PxTransform& camera)
{
    PX_UNUSED(camera); // No se utiliza la variable 'camera'

    switch (toupper(key)) // Convierte el car�cter a may�scula y eval�a
    {
        //case 'B': break;
        //case ' ':	break;
    case ' ':
    {
        break;
    }
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
    extern void renderLoop(); // Declaraci�n de una funci�n externa llamada renderLoop
    renderLoop(); // Llama a la funci�n renderLoop
#else
    static const PxU32 frameCount = 100; // N�mero de fotogramas para la ejecuci�n fuera de l�nea
    initPhysics(false); // Inicializa la f�sica en modo fuera de l�nea
    for (PxU32 i = 0; i < frameCount; i++)
        stepPhysics(false); // Realiza pasos de f�sica en modo fuera de l�nea
    cleanupPhysics(false); // Limpia los datos en modo fuera de l�nea
#endif

    return 0; // Devuelve 0 como c�digo de salida
}
