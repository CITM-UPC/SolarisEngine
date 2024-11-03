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

    RenderContext();

    ImGui::End(); // Finaliza el panel
}

void PanelHierarchy::RenderContext()
{

    GameObject* gameObjectSelected = app->actualScene->GetSelectedGameObject();

    if (ImGui::BeginPopupContextItem("GameObjectContextMenu")) {
        if (ImGui::MenuItem("Create Empty")) {
            auto newGO = GameObject::Create("New GameObject");
            app->actualScene->AddGameObject(newGO);
        }

        if (ImGui::MenuItem("Duplicate")) {
            if (gameObjectSelected) {
                auto duplicateGO = gameObjectSelected->Duplicate();
                app->actualScene->AddGameObject(duplicateGO);
            }
        }

        if (ImGui::MenuItem("Delete")) {
            if (gameObjectSelected) {
                app->actualScene->RemoveGameObject(gameObjectSelected);
            }
        }

        if (ImGui::MenuItem("Copy")) {
            // Lógica para copiar el GameObject
            // Guardar en una variable estática o similar
            app->actualScene->SetCopiedGameObject(gameObjectSelected);
        }

        if (ImGui::MenuItem("Paste")) {
            if (app->actualScene->GetCopiedGameObject()) {
                auto newGO = app->actualScene->GetCopiedGameObject()->Duplicate(); // Duplica el GameObject copiado
                app->actualScene->AddGameObject(newGO);
            }
        }

        ImGui::EndPopup();
    }
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
