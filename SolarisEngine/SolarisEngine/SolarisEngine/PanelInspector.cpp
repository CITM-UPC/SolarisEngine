#include "PanelInspector.h"
#include "imgui.h"
#include "App.h"

PanelInspector::PanelInspector() {
    // Inicialización si es necesaria
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
       

        ImGui::Text("Inspector for: %s", selectedGameObject->GetName().c_str()); // Muestra el nombre del GameObject seleccionado
        // Checkbox para habilitar/deshabilitar el GameObject
        bool isEnabled = selectedGameObject->IsEnabled(); // Obtén el estado actual
        if (ImGui::Checkbox("Enabled", &isEnabled)) { // Usa una variable temporal
            selectedGameObject->SetEnable(isEnabled); // Cambia el estado
        }


        // Renderiza los componentes del GameObject
        for (auto component : selectedGameObject->GetComponents()) { // Asume que tienes un método GetComponents()
            //DrawComponent(component);
            /*if (ImGui::CollapsingHeader(component->GetName().c_str())) {
                component->DrawInspectorComponent();
            }*/
            component->DrawInspectorComponent();
        }
    }
    else {
        ImGui::Text("No GameObject selected."); // Mensaje si no hay ningún GameObject seleccionado
    }

    ImGui::End(); // Finaliza el panel
}

void PanelInspector::RenderContext()
{
}
