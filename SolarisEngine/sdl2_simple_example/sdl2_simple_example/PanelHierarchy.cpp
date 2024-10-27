#include "PanelHierarchy.h"
#include "imgui.h"
#include "App.h"
#include "PanelInspector.h" // Asegúrate de incluir el header para el PanelInspector

PanelHierarchy::PanelHierarchy()
    : selectedGameObject(nullptr) { // Inicializa el puntero seleccionado a nullptr
}

PanelHierarchy::~PanelHierarchy() {
    // Limpieza si es necesaria
}

void PanelHierarchy::Render() {
    ImGui::Begin("Hierarchy"); // Nombre del panel

    // Renderiza cada GameObject en la lista
    for (auto gameObject : app->gameObjects) {
        DrawGameObject(gameObject);
    }

    ImGui::End(); // Finaliza el panel
}

void PanelHierarchy::DrawGameObject(GameObject* gameObject) {
    ImGui::Text(gameObject->GetName().c_str()); // Dibuja el nombre del GameObject

    // Detecta clic en el GameObject
    if (ImGui::IsItemClicked()) {
        selectedGameObject = gameObject; // Actualiza el GameObject seleccionado
        app->windowEditor->_windowImGui->inspectorPanel->SetSelectedGameObject(selectedGameObject); // Notifica al PanelInspector
    }
}
