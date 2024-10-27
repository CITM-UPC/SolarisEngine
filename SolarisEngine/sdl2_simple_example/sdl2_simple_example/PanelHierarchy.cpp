#include "PanelHierarchy.h"
#include "imgui.h"

PanelHierarchy::PanelHierarchy() {
    // Inicializaci�n si es necesaria
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
        gameObjects.push_back(gameObject); // A�ade el GameObject a la lista
    }
}

void PanelHierarchy::DrawGameObject(GameObject* gameObject) {
    // Dibuja el nombre del GameObject
    ImGui::Text(gameObject->GetName().c_str());

    // Puedes a�adir m�s interactividad, como un bot�n para seleccionar o eliminar el GameObject
    if (ImGui::IsItemClicked()) {
        // Acci�n cuando se hace clic en el GameObject (ejemplo: seleccionarlo)
    }
}
