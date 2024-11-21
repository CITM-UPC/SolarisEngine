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


    GameObject* selectedGameObject = app->actualScene->GetSelectedGameObject();

    if (selectedGameObject) {
       

        // Muestra el nombre del GameObject seleccionado
        if (selectedGameObject->parent) {
            ImGui::Text("Inspector for: %s, (P: %s)", selectedGameObject->GetName().c_str(), selectedGameObject->parent->GetName().c_str()); 
        }
        else {
            ImGui::Text("Inspector for: %s", selectedGameObject->GetName().c_str()); 
        }
      




        // Checkbox para habilitar/deshabilitar el GameObject
        bool isEnabled = selectedGameObject->IsEnabled(); // Obt�n el estado actual
        if (ImGui::Checkbox("Enabled", &isEnabled)) { // Usa una variable temporal
            selectedGameObject->SetEnable(isEnabled); // Cambia el estado
        }
        ImGui::SameLine();
        bool isStatic = selectedGameObject->IsStatic(); // Obt�n el estado actual
        if (ImGui::Checkbox("Static", &isStatic)) { // Usa una variable temporal
            selectedGameObject->SetStatic(isStatic); // Cambia el estado
        }


        // Renderiza los componentes del GameObject
        for (auto component : selectedGameObject->GetComponents()) { // Asume que tienes un m�todo GetComponents()
            //DrawComponent(component);
            /*if (ImGui::CollapsingHeader(component->GetName().c_str())) {
                component->DrawInspectorComponent();
            }*/
            component->DrawInspectorComponent();
        }
    }
    else {
        ImGui::Text("No GameObject selected."); // Mensaje si no hay ning�n GameObject seleccionado
    }

    ImGui::End(); // Finaliza el panel
}

void PanelInspector::RenderContext()
{
}
