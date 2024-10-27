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
    hierarchyPanel = new PanelHierarchy(); // Crea el panel de jerarquía
    panels.push_back(hierarchyPanel); // Añade el panel a la lista de panels

    inspectorPanel = new PanelInspector(); // Crea el panel del inspector
    panels.push_back(inspectorPanel); // Añade el panel a la lista de panels
}

void WindowImGui::Create() {
    // Aquí puedes realizar inicializaciones adicionales si es necesario
    // Por ejemplo, puedes inicializar otros paneles si tienes más
}

void WindowImGui::Shutdown() {
    // Aquí puedes realizar cualquier limpieza adicional si es necesario
}

void WindowImGui::HandleSDLEvent(SDL_Event& e) {
    // Aquí puedes procesar eventos específicos de SDL para ImGui
    // Ejemplo: ImGui_ImplSDL2_ProcessEvent(&e);
}

void WindowImGui::Render() {
    // Renderiza cada panel en la lista
    menuBar->Render();

    for (auto panel : panels) {
        panel->Render(); // Llama al método de renderizado de cada panel
    }
}
