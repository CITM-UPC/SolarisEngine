#ifndef __APP_H__
#define __APP_H__

#include <string>
#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/ext/vector_int2.hpp>
#include <exception>
#include <thread>

#include "MyWindow.h"
#include "Importer.h"

#include "CameraEditor.h"
#include "Input.h"
#include "WindowEditor.h"
#include "TextureLoader.h"

#include "Scene.h"

#include "Primitivos.h"
#include "Component_Material.h"
#include "Component_Transform.h"
#include "Component_BoundingBox.h"
#include "Component_Billboard.h"
#include "Component_ParticleSystem.h"

// Classes "globales"
class MyWindow;
class Importer;
class CameraEditor;
class InputEditor;
class WindowEditor;
class TextureLoader;
class Scene;

class App
{
public:

    // Constructor
    App(int argc, char* args[]);

    // Destructor
    virtual ~App();

    // Called before render is available
    bool Awake();

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool Update();

    // Called before quitting
    bool CleanUp();

    // Exposing some properties for reading
    int GetArgc() const;
    const char* GetArgv(int index) const;
    const char* GetTitle() const;
    const char* GetOrganization() const;

    // Request a save data in an XML file 
    bool LoadRequest();

    // Request to load data from XML file 
    bool SaveRequest();

    bool IsSaving();

private:

    // Load config file
    bool LoadConfig();

    // Call modules before each loop iteration
    void PrepareUpdate();

    // Call modules before each loop iteration
    void FinishUpdate();

    // Call modules before each loop iteration
    bool PreUpdate();

    // Call modules on each loop iteration
    bool DoUpdate(double dt);

    // Call modules after each loop iteration
    bool PostUpdate(double dt);
    bool PostLateUpdate(double dt);

    // Reads XML file and loads the data
    bool LoadFromFile();

    // Save XML file with modules data
    bool SaveFromFile();

    bool INIT_openGL();

    bool HandleEvents();

public:

    double dt;

    double contador = 0;

    Importer* importer;
    CameraEditor* cameraEditor;
    MyWindow* myWindow;
    InputEditor* inputEditor;
    WindowEditor* windowEditor;
    TextureLoader* textureLoader;

    Scene* actualScene;

    static inline const glm::ivec2 WINDOW_SIZE = { 1024, 720 };
    static const unsigned int FPS = 60;
    static inline const auto FRAME_DT = 1.0s / FPS;

    GameObject* gameObject;  // Cambiado a puntero crudo
    GameObject* gameObject2; // Cambiado a puntero crudo
    GameObject* gameObject3; // Cambiado a puntero crudo



private:

    int argc;
    char** args;
    std::string gameTitle;
    std::string organization;

    bool loadRequest = false;
    bool saveRequest = false;
    bool isSaving = false;
};

extern App* app;

#endif // __APP_H__
