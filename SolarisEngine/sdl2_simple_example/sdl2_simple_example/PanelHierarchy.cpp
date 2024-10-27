#include "PanelHierarchy.h"
#include "imgui.h"

PanelHierarchy::PanelHierarchy() {
    // Inicialización si es necesaria
}

PanelHierarchy::~PanelHierarchy() {
    // Limpieza si es necesaria
}

void PanelHierarchy::Render() {


 


    ImGui::Begin("Hierarchy"); // Nombre del panel

    // Renderiza cada GameObject en la lista
    for (auto gameObject : gameObjects) {
        DrawGameObject(gameObject);
    }

    ImGui::End(); // Finaliza el panel
}

void PanelHierarchy::AddGameObject(GameObject* gameObject) {
    if (gameObject) {
        gameObjects.push_back(gameObject); // Añade el GameObject a la lista
    }
}

void PanelHierarchy::DrawGameObject(GameObject* gameObject) {
    // Dibuja el nombre del GameObject
    ImGui::Text(gameObject->GetName().c_str());

    // Puedes añadir más interactividad, como un botón para seleccionar o eliminar el GameObject
    if (ImGui::IsItemClicked()) {
        // Acción cuando se hace clic en el GameObject (ejemplo: seleccionarlo)
    }
}
