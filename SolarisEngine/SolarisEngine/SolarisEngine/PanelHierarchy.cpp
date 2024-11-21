#include "PanelHierarchy.h"
#include "imgui.h"
#include "App.h"
#include "PanelInspector.h" 
#include "Input.h"

PanelHierarchy::PanelHierarchy() {
    selectedItem = "";
}

PanelHierarchy::~PanelHierarchy() {}

void PanelHierarchy::Render() {
    if (!app->windowEditor->GetImGuiWindow()->showHierarchyPanel) {
        return;
    }

    ImGui::Begin("Hierarchy"); // Nombre del panel

    UpdateHoveredStatus();

    // Renderizar recursivamente cada GameObject y sus hijos
    RenderGameObjectsRecursively(app->actualScene->GetGameObjectsList());

    RenderContext();

    if (isHovered && app->inputEditor->mouseLeftIsPressed &&
        !ImGui::IsAnyItemHovered() && !app->inputEditor->isCameraMoving) {
        app->actualScene->SelectGameObject(nullptr);
    }

    ImGui::End(); // Finaliza el panel
}
void PanelHierarchy::RenderGameObjectsRecursively(const std::vector<GameObject*> gameObjects, int indentLevel) {
    for (auto& gameObject : gameObjects) {
        bool isSelected = (gameObject == app->actualScene->GetSelectedGameObject());

        // Crear un ID único para cada GameObject en el panel
        ImGui::PushID(gameObject);

        // Detectar si el GameObject tiene hijos
        bool hasChildren = !gameObject->GetChildren().empty();

        // Indentar la posición dependiendo del nivel
        ImGui::Indent(indentLevel * 20.0f);

        // Mostrar un botón para expandir o contraer los hijos (fuera de la Selectable)
        bool isExpanded = gameObject->isExpanded; // Suponemos que 'isExpanded' está gestionado en el GameObject
        if (hasChildren) {
            // Mostrar flecha solo si tiene hijos
            if (ImGui::ArrowButton(("##expand_" + gameObject->GetName()).c_str(), isExpanded ? ImGuiDir_Down : ImGuiDir_Right)) {
                gameObject->isExpanded = !isExpanded; // Cambiar estado de expansión
            }

            // Añadir un pequeño espacio después de la flecha
            ImGui::SameLine();
        }

        // Mostrar el nombre del GameObject de manera separada de la flecha
        std::string gameObjectName = u8"\ue079 " + gameObject->GetName(); // Puedes cambiar el ícono por algo más adecuado
        if (ImGui::Selectable(gameObjectName.c_str(), isSelected)) {
            app->actualScene->SelectGameObject(gameObject);
        }

        // Habilitar el arrastre
        if (ImGui::BeginDragDropSource()) {
            // Establecer el payload con el puntero del GameObject arrastrado
            ImGui::SetDragDropPayload("GAMEOBJECT", &gameObject, sizeof(GameObject*));
            ImGui::Text("Arrastrando %s", gameObject->GetName().c_str());
            ImGui::EndDragDropSource();
        }

        // Soporte para soltar sobre otro GameObject
        if (ImGui::BeginDragDropTarget()) {
            const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GAMEOBJECT");
            if (payload != nullptr) {
                GameObject* draggedObject = *(GameObject**)payload->Data;
                if (gameObject != draggedObject) {
                    // Cambiar el padre del GameObject arrastrado
                    draggedObject->SetParent(gameObject);
                }
            }
            ImGui::EndDragDropTarget();
        }

        // Llamada recursiva para los hijos si están expandidos
        if (gameObject->isExpanded) {
            RenderGameObjectsRecursively(gameObject->GetChildren(), indentLevel + 1);
        }

        ImGui::Unindent(indentLevel * 20.0f); // Restaurar la indentación
        ImGui::PopID(); // Pop ID para este GameObject
    }
}

void PanelHierarchy::DrawGameObject(GameObject* gameObject) {
    ImGuiIO& io = ImGui::GetIO();

    std::string gameObjectName = u8"\ue079 " + gameObject->GetName();
    bool isSelected = (app->actualScene->GetSelectedGameObject() == gameObject);
    bool isHovered = false;

    if (isSelected) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_Text, gameObject->IsEnabled() ?
            ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
    }

    ImGui::PushStyleColor(ImGuiCol_Button, isSelected ? ImVec4(0.0f, 0.0f, 1.0f, 0.5f) : ImVec4(0, 0, 0, 0));

    // Detectar click izquierdo
    if (ImGui::Button(gameObjectName.c_str())) {
        app->actualScene->SelectGameObject(gameObject);
    }

    if (ImGui::IsItemHovered()) {
        isHovered = true;
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 1.0f, 0.5f));
    }

    if (ImGui::IsItemClicked(0)) { // Click izquierdo
        app->actualScene->SelectGameObject(gameObject);
    }

    if (ImGui::IsItemClicked(1)) { // Click derecho
        app->actualScene->SelectGameObject(gameObject);
        ImGui::OpenPopup("HierarchyContextMenu");
    }

    // Restaurar colores
    if (isHovered) {
        ImGui::PopStyleColor();
    }

    ImGui::PopStyleColor(); // Restaurar color del botón
    ImGui::PopStyleColor(); // Restaurar color del texto
}

void PanelHierarchy::RenderContext() {
    GameObject* gameObjectSelected = app->actualScene->GetSelectedGameObject();

    if (ImGui::BeginPopupContextWindow("HierarchyContextMenu", 1)) {
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

        if (ImGui::MenuItem("Move Up")) {
            if (gameObjectSelected) {
                app->actualScene->MoveGameObjectUp(gameObjectSelected);
            }
        }

        if (ImGui::MenuItem("Move Down")) {
            if (gameObjectSelected) {
                app->actualScene->MoveGameObjectDown(gameObjectSelected);
            }
        }

        ImGui::EndPopup();
    }
}

void PanelHierarchy::UpdateHoveredStatus() {
    // Implementa cualquier lógica necesaria para el estado hover
}
