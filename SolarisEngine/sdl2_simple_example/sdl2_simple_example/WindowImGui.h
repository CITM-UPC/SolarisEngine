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

    void Create(); // M�todo para inicializar cualquier recurso
    void Shutdown(); // M�todo para liberar recursos
    void HandleSDLEvent(SDL_Event& e); // Manejar eventos de SDL
    void Render(); // M�todo para renderizar todos los paneles
    void AddGameObjectToHierarchy(GameObject* gameObject); // A�adir GameObjects al panel de jerarqu�a

private:
    void CreatePanels(); // M�todo para inicializar los paneles
    std::vector<PanelHierarchy*> panels; // Lista de paneles
    PanelHierarchy* hierarchyPanel; // Panel de jerarqu�a
    MenuBar* menuBar;
};
