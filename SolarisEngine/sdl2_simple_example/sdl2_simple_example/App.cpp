#include "Debug.h"
#include <GL/glew.h> // Asegúrate de incluir GLEW antes de OpenGL
#include <GL/gl.h>

#include "App.h"
#include "Defs.h"
#include <iostream>
#include <exception>
#include <imgui_impl_sdl2.h>
#include <SDL2/SDL_events.h>



App* app = NULL;

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args), loadRequest(false), saveRequest(false), isSaving(false)
{
    // Inicializar el título del juego y la organización
    gameTitle = "My Game";
    organization = "My Organization";

    srand(static_cast<unsigned int>(time(0)));
}

// Destructor
App::~App()
{
    // Llamar a CleanUp antes de destruir la aplicación
    CleanUp();
}

// Called before render is available
bool App::Awake()
{

    bool ret = LoadConfig();

    std::cout << "App::Awake called" << std::endl;
    Debug::Log("App::Awake called");
    // Aquí podrías cargar la configuración del juego

    return ret;
}

// Called before the first frame
bool App::Start()
{
    std::cout << "App::Start called" << std::endl;
    Debug::Log("App::Start called");

    // Crear los objetos con new
    gameObject = importer->Importar("./Assets/BakerHouse.fbx"); // <-- Modelo con 1 textura
    gameObject->AddComponent<Component_Material>()->SetTexture("./Assets/Baker_house.png");
    gameObject2 = importer->Importar("./Assets/Sora.fbx", "./Assets/Mat0.png"); // <-- Modelo que varias texturas
    gameObject2->GetComponent<Component_Transform>()->SetRotation(-90,0,0);


    actualScene->AddGameObject(gameObject);
    actualScene->AddGameObject(gameObject2);

    gameObject = importer->Importar("./Assets/Potato.fbx");
    gameObject->AddComponent<Component_Material>()->SetTexture("./Assets/Potato.jpg");
    gameObject->GetComponent<Component_Transform>()->SetPosition(0, -2, 0);
    actualScene->AddGameObject(gameObject);


    return true;
}

// Called each loop iteration
bool App::Update()
{
    std::cout << "App::Update called" << std::endl;
    Debug::Log("App::Update called");
    
    PrepareUpdate();

    while (HandleEvents()) {
        const auto t0 = hrclock::now();

        if (!PreUpdate()) return false;
        if (!DoUpdate(dt)) return false;
        if (!PostUpdate(dt)) return false;

        

        if (!PostLateUpdate(dt)) return false;

        const auto t1 = hrclock::now();
        dt = std::chrono::duration_cast<std::chrono::duration<double>>(t1 - t0).count();

        if (dt < FRAME_DT.count()) {
            std::this_thread::sleep_for(FRAME_DT - std::chrono::duration<double>(dt));
        }

        windowEditor->Render(); // Renderiza ImGui aquí
    }

    FinishUpdate();

    return false;
}

// Called before quitting
bool App::CleanUp()
{
    std::cout << "App::CleanUp called" << std::endl;
    Debug::Log("App::CleanUp called");

   
    // Eliminar los objetos con delete
    if (gameObject) {
        gameObject->Delete();
        gameObject = nullptr;
    }

    if (gameObject2) {
        gameObject2->Delete();
        gameObject2 = nullptr;
    }

    if (gameObject3) {
        gameObject3->Delete();
        gameObject3 = nullptr;
    }

    // Limpiar y liberar recursos antes de salir
    return true;
}

// Getters
int App::GetArgc() const
{
    return argc;
}

const char* App::GetArgv(int index) const
{
    if (index < argc) {
        return args[index];
    }
    return nullptr;
}

const char* App::GetTitle() const
{
    return gameTitle.c_str();
}

const char* App::GetOrganization() const
{
    return organization.c_str();
}

// Requests
bool App::LoadRequest()
{
    loadRequest = true;
    return loadRequest;
}

bool App::SaveRequest()
{
    saveRequest = true;
    return saveRequest;
}

bool App::IsSaving()
{
    return isSaving;
}

// Private methods

bool App::LoadConfig()
{

    
    textureLoader = new TextureLoader();

    windowEditor = new WindowEditor();
    windowEditor->Create();


    importer = &Importer::getInstance();
    cameraEditor = new CameraEditor(
        glm::vec3(0.0f, 0.3f, 0.2f),    // Posición inicial
        glm::vec3(0.0f, 0.0f, -1.0f),   // Dirección de la cámara
        glm::vec3(0.0f, 1.0f, 0.0f));   // Vector up

    inputEditor = new InputEditor();
   
    
    actualScene = new Scene();

   
    INIT_openGL();



    return true;
}

void App::PrepareUpdate()
{
    // Preparar la lógica antes de actualizar cada módulo
}

void App::FinishUpdate()
{
    // Finalizar la lógica después de actualizar cada módulo
}

bool App::PreUpdate()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    return true;
}

bool App::DoUpdate(double dt)
{
    cameraEditor->Update();

    actualScene->Update(dt);

    //if (gameObject) {
    //   /* Component_Transform* ct = gameObject->GetComponent<Component_Transform>();
    //    ct->SetPosition(ct->GetPosition().x + 0.01f, ct->GetPosition().y, ct->GetPosition().z);
    //    ct->SetScale(ct->GetScale().x, ct->GetScale().y + 0.01f, ct->GetScale().z);*/
    //    gameObject->Draw();
    //}




    //if (gameObject2) {
    //    Component_Transform* ct = gameObject2->GetComponent<Component_Transform>();
    //    ct->SetRotation(ct->GetRotation().x + 1, 0, 0);
    //    gameObject2->Draw();
    //}

   

    //if (gameObject3) {
    //    gameObject3->Draw();
    //}

  


    return true;
}

bool App::PostUpdate(double dt)
{
    return true;
}

bool App::PostLateUpdate(double dt)
{
    
    return true;
}

bool App::LoadFromFile()
{
    return true;
}

bool App::SaveFromFile()
{
    return true;
}

bool App::INIT_openGL() {

    Debug::Log("Inicializando OpenGL");

    glewInit();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glClearColor(0.5, 0.5, 0.5, 1.0);


    return true;
}



bool App::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Manejar el evento de salida
        if (event.type == SDL_QUIT) {
            return false; // Salir de la aplicación
        }

        // Delegar el evento al InputEditor
        if (!inputEditor->processEvents(event)) {
            return false; // Si processEvents devuelve false, salir
        }

        // Delegar el evento al WindowEditor
        if (!windowEditor->PumpEvents(event)) {
            return false; // Si PumpEvents devuelve false, salir
        }
    }
    return true; // Todo bien
}