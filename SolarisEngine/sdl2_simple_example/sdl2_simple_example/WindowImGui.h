#pragma once

#include <vector>
#include <string>
#include "PanelHierarchy.h" // Incluye tu clase PanelHierarchy
#include "MenuBar.h" // Incluye tu clase PanelHierarchy
#include <imgui_impl_sdl2.h>

class WindowImGui {
public:
    WindowImGui();
    ~WindowImGui();

    void Create(); // Método para inicializar cualquier recurso
    void Shutdown(); // Método para liberar recursos
    void HandleSDLEvent(SDL_Event& e); // Manejar eventos de SDL
    void Render(); // Método para renderizar todos los paneles
    void AddGameObjectToHierarchy(GameObject* gameObject); // Añadir GameObjects al panel de jerarquía

private:
    void CreatePanels(); // Método para inicializar los paneles
    std::vector<PanelHierarchy*> panels; // Lista de paneles
    PanelHierarchy* hierarchyPanel; // Panel de jerarquía
    MenuBar* menuBar;
};
