#include "PanelHierarchy.h"
#include "imgui.h"
#include "App.h"
#include "PanelInspector.h" // Asegúrate de incluir el header para el PanelInspector
#include "Input.h"

PanelHierarchy::PanelHierarchy() { // Inicializa el puntero seleccionado a nullptr
    selectedItem = ""; // item selecionado
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
    // 获取 ImGui 的 IO
    ImGuiIO& io = ImGui::GetIO();

    // 确定图标和名称
    std::string gameObjectName = u8"\ue079 " + gameObject->GetName();
    bool isSelected = (app->actualScene->GetSelectedGameObject() == gameObject);
    bool isHovered = false;

    ImGui::BeginGroup();

    // 设置文本颜色
    if (isSelected) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_Text, gameObject->IsEnabled() ?
            ImVec4(1.0f, 1.0f, 1.0f, 1.0f) :
            ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
    }

    // 如果选中，背景颜色变为深蓝色
    if (isSelected) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 0.5f));
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    }

    if (ImGui::Button(gameObjectName.c_str())) {
        app->actualScene->SelectGameObject(gameObject);
    }

    // 悬停时变色
    if (ImGui::IsItemHovered()) {
        isHovered = true;
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 1.0f, 0.5f));
    }

    // 检测左键点击
    if (ImGui::IsItemClicked(0)) { // 左键点击
        app->actualScene->SelectGameObject(gameObject);
    }

    // 检测右键点击
    if (ImGui::IsItemClicked(1)) { // 右键点击
        app->actualScene->SelectGameObject(gameObject); // 选择GameObject
        ImGui::OpenPopup("HierarchyContextMenu"); // 打开弹出菜单
    }

     /*检测点击其他地方*/
    if (app->inputEditor->mouseLeftIsPressed && !app->inputEditor->isCameraMoving && !ImGui::IsItemHovered() && !ImGui::IsAnyItemHovered()) {
        app->actualScene->SelectGameObject(nullptr); // 取消选择
    }

    // 悬停时变色恢复
    if (isHovered) {
        ImGui::PopStyleColor();
    }

    ImGui::PopStyleColor(); // 恢复按钮颜色
    ImGui::PopStyleColor(); // 恢复文本颜色
    ImGui::EndGroup();
}


/* bool isHovered = false; bool isSelected = (gameObjectName == selectedItem); 
ImGui::PushStyleColor(ImGuiCol_Button, isSelected ? ImVec4(0.0f, 0.0f, 1.0f, 0.5f) : ImVec4(0, 0, 0, 0));*/

