#include "PanelHierarchy.h"
#include "imgui.h"
#include "App.h"

PanelHierarchy::PanelHierarchy() {
    // Inicializaci�n si es necesaria
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

    // Puedes a�adir m�s interactividad, como un bot�n para seleccionar o eliminar el GameObject
    if (ImGui::IsItemClicked()) {
        // Acci�n cuando se hace clic en el GameObject (ejemplo: seleccionarlo)
        ImGui::Text("Selected: %s", gameObject->GetName().c_str());
    }
}

