#include "PanelHierarchy.h"
#include "imgui.h"
#include "App.h"
#include "PanelInspector.h" // Asegúrate de incluir el header para el PanelInspector

PanelHierarchy::PanelHierarchy() { // Inicializa el puntero seleccionado a nullptr
}

PanelHierarchy::~PanelHierarchy() {
    // Limpieza si es necesaria
}

void PanelHierarchy::Render() {

    if (!app->windowEditor->GetImGuiWindow()->showHierarchyPanel) {
        return;
    }

    ImGui::Begin("Hierarchy"); // Nombre del panel

    // Renderiza cada GameObject en la lista
    for (auto gameObject : app->actualScene->GetGameObjectsList()) {
        DrawGameObject(gameObject);
    }

    ImGui::End(); // Finaliza el panel
}

void PanelHierarchy::DrawGameObject(GameObject* gameObject) {

    // Dibuja el nombre del GameObject y aplica color al texto si está seleccionado
    if (app->actualScene->GetSelectedGameObject() == gameObject) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
    }
    else {
        if (gameObject->IsEnabled()) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); 
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
        }
       
    }



    std::string gameObjectName = u8"\ue079 " + gameObject->GetName();
    ImGui::Text(gameObjectName.c_str()); // Dibuja el nombre del GameObject



    // Detecta clic en el GameObject
    if (ImGui::IsItemClicked()) {
        app->actualScene->SelectGameObject(gameObject);
    }

    
    
    ImGui::PopStyleColor();
   

  
}
