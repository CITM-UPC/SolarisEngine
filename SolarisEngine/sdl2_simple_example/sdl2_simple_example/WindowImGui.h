#pragma once

#include <vector>
#include <string>
#include <imgui_impl_sdl2.h>
#include "MenuBar.h" 
#include "PanelHierarchy.h" 
#include "PanelInspector.h"
#include "PanelConsola.h"
#include "PanelProject.h"

class WindowImGui {
public:
    WindowImGui();
    ~WindowImGui();

    void Create(); // Método para inicializar cualquier recurso
    void Shutdown(); // Método para liberar recursos
    void HandleSDLEvent(SDL_Event& e); // Manejar eventos de SDL
    void Render(); // Método para renderizar todos los paneles

    PanelHierarchy* hierarchyPanel; // Panel de jerarquía
    PanelInspector* inspectorPanel; // Panel del inspector
    PanelConsola* consolaPanel; // Panel del consola
    PanelProject* projectPanel; // Panel del proyecto
    MenuBar* menuBar;

    bool showHierarchyPanel = true;
    bool showinspectorPanel = true;
    bool showconsolaPanel = true;
    bool showprojectPanel = true;

 


private:
    void CreatePanels(); // Método para inicializar los paneles
    std::vector<Panel*> panels; // Lista de paneles
   

};
