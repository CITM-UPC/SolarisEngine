#include "WindowImGui.h"
#include "imgui.h"

#include <SDL2/SDL.h>

WindowImGui::WindowImGui()
    : hierarchyPanel(nullptr), inspectorPanel(nullptr) {
    CreatePanels(); // Crea los paneles al inicializar
}

WindowImGui::~WindowImGui() {
    // Limpieza de los paneles
    for (auto panel : panels) {
        delete panel;
    }
    panels.clear();
}

void WindowImGui::CreatePanels() {
    hierarchyPanel = new PanelHierarchy(); // Crea el panel de jerarqu�a
    panels.push_back(hierarchyPanel); // A�ade el panel a la lista de panels

    inspectorPanel = new PanelInspector(); // Crea el panel del inspector
    panels.push_back(inspectorPanel); // A�ade el panel a la lista de panels
}

void WindowImGui::Create() {
    // Aqu� puedes realizar inicializaciones adicionales si es necesario
    // Por ejemplo, puedes inicializar otros paneles si tienes m�s
}

void WindowImGui::Shutdown() {
    // Aqu� puedes realizar cualquier limpieza adicional si es necesario
}

void WindowImGui::HandleSDLEvent(SDL_Event& e) {
    // Aqu� puedes procesar eventos espec�ficos de SDL para ImGui
    // Ejemplo: ImGui_ImplSDL2_ProcessEvent(&e);
}

void WindowImGui::Render() {
    // Renderiza cada panel en la lista
    menuBar->Render();

    for (auto panel : panels) {
        panel->Render(); // Llama al m�todo de renderizado de cada panel
    }
}
