#include "PanelInspector.h"
#include "imgui.h"
#include "App.h"

PanelInspector::PanelInspector() {
    // Inicializaci�n si es necesaria
}

PanelInspector::~PanelInspector() {
    // Limpieza si es necesaria
}

void PanelInspector::Render() {

    if (!app->windowEditor->GetImGuiWindow()->showinspectorPanel) {
        return;
    }

    ImGui::Begin("Inspector"); // Nombre del panel

    if (app->actualScene->GetSelectedGameObject()) {

        GameObject* selectedGameObject = app->actualScene->GetSelectedGameObject();

        ImGui::Text("Inspector for: %s", selectedGameObject->GetName().c_str()); // Muestra el nombre del GameObject seleccionado

        // Renderiza los componentes del GameObject
        for (auto component : selectedGameObject->GetComponents()) { // Asume que tienes un m�todo GetComponents()
            //DrawComponent(component);
            if (ImGui::CollapsingHeader(component->GetName().c_str())) {
                component->DrawInspectorComponent();
            }
        }
    }
    else {
        ImGui::Text("No GameObject selected."); // Mensaje si no hay ning�n GameObject seleccionado
    }

    ImGui::End(); // Finaliza el panel
}
