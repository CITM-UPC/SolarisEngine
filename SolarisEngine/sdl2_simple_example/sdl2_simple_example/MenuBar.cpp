#include "MenuBar.h"
#include <SDL2/SDL.h>
#include "App.h"

void MenuBar::Render() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Menu")) {
            if (ImGui::MenuItem("Exit")) {
                SDL_Event quit_event;
                quit_event.type = SDL_QUIT;
                SDL_PushEvent(&quit_event);
            }
            if (ImGui::MenuItem("ShowDemo")) {
                showDemo = !showDemo;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Primitive")) {
            if (ImGui::MenuItem("Plane")) {
                auto newGO = GameObject::Create("Plane");
                auto mesh = newGO->AddComponent<Component_Mesh>();
                mesh->GeneratePlaneMesh();
                app->gameObjects.push_back(newGO);  // Agregar a la lista global de GameObjects
            }
            if (ImGui::MenuItem("Cube")) {
                auto newGO = GameObject::Create("Cube");
                auto mesh = newGO->AddComponent<Component_Mesh>();
                mesh->GenerateCubeMesh();
                app->gameObjects.push_back(newGO);
            }
            if (ImGui::MenuItem("Triangle")) {
                auto newGO = GameObject::Create("Triangle");
                auto mesh = newGO->AddComponent<Component_Mesh>();
                mesh->GenerateTriangleMesh();
                app->gameObjects.push_back(newGO);
            }
            if (ImGui::MenuItem("Sphere")) {
                auto newGO = GameObject::Create("Sphere");
                auto mesh = newGO->AddComponent<Component_Mesh>();
                mesh->GenerateSphereMesh();
                app->gameObjects.push_back(newGO);
            }
            if (ImGui::MenuItem("Capsule")) {
                auto newGO = GameObject::Create("Capsule");
                auto mesh = newGO->AddComponent<Component_Mesh>();
                mesh->GenerateCapsuleMesh();
                app->gameObjects.push_back(newGO);
            }
            if (ImGui::MenuItem("Cylinder")) {
                auto newGO = GameObject::Create("Cylinder");
                auto mesh = newGO->AddComponent<Component_Mesh>();
                mesh->GenerateCylinderMesh();
                app->gameObjects.push_back(newGO);
            }

            ImGui::EndMenu();
        }



        

        ImGui::EndMainMenuBar();
    }

    if (showDemo) {
            ImGui::ShowDemoWindow();
     }
    
}
