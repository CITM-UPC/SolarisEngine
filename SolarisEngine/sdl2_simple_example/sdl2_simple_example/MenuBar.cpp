#include "MenuBar.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "App.h"
#include "imgui.h"
#include <vector>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <psapi.h>  // Para GetProcessMemoryInfo en Windows
#elif defined(__linux__)
#include <unistd.h>
#include <fstream>
#elif defined(__APPLE__)
#include <mach/mach.h>
#endif

MenuBar::MenuBar()
    : showDemo(false), fpsHistory{}, currentFPS(0.0f), frameCounter(0), showOverlayFPS(false) {} // Añadido showOverlayFPS

size_t MenuBar::GetMemoryUsage() {
#if defined(_WIN32) || defined(_WIN64)
    // En Windows, obtenemos la memoria utilizada por el proceso actual
    PROCESS_MEMORY_COUNTERS_EX memInfo;
    if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&memInfo, sizeof(memInfo))) {
        return memInfo.PrivateUsage / (1024 * 1024);  // Devuelve en MB
    }
    return 0;

#elif defined(__linux__)
    // En Linux, usamos /proc/self/statm para obtener el uso de memoria del proceso actual
    std::ifstream statm("/proc/self/statm");
    size_t memory = 0;
    if (statm.is_open()) {
        statm >> memory;  // Leer el tamaño de memoria residente
        memory *= sysconf(_SC_PAGESIZE);  // Convertir a bytes
    }
    return memory / (1024 * 1024);  // Devuelve en MB

#elif defined(__APPLE__)
    // En macOS, obtenemos el uso de memoria del proceso mediante task_info
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &infoCount) == KERN_SUCCESS) {
        return info.resident_size / (1024 * 1024);  // Devuelve en MB
    }
    return 0;
#else
    return 0;  // No implementado para otros sistemas
#endif
}

void MenuBar::Render() {
    // Incrementar el contador de frames
    frameCounter++;

    // Calcular FPS y actualizar historial cada 10 frames
    if (frameCounter >= 10) {
        float fps = 1.0f / ImGui::GetIO().DeltaTime;
        fpsHistory.push_back(fps);
        if (fpsHistory.size() > 100) {
            fpsHistory.erase(fpsHistory.begin());  // Limitar historial de FPS
        }
        currentFPS = fps;
        frameCounter = 0;  // Reiniciar contador
    }

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("General")) {
            if (ImGui::MenuItem("Exit")) {
                SDL_Event quit_event;
                quit_event.type = SDL_QUIT;
                SDL_PushEvent(&quit_event);
            }
            if (ImGui::MenuItem("About")) {
                SDL_OpenURL("https://github.com/Solaris-Group/SolarisEngine");
            }
            if (ImGui::MenuItem("ShowDemo")) {
                showDemo = !showDemo;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Assets")) {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("GameObject")) {
            if (ImGui::BeginMenu("Primitivos")) {
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
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Componentes")) {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Ventana")) {

            ImGui::MenuItem("Inspector", nullptr, &app->windowEditor->GetImGuiWindow()->showinspectorPanel); // Opción para mostrar/ocultar Inspector
            ImGui::MenuItem("Consola", nullptr, &app->windowEditor->GetImGuiWindow()->showconsolaPanel); // Opción para mostrar/ocultar Consola
            ImGui::MenuItem("Jerarquía", nullptr, &app->windowEditor->GetImGuiWindow()->showHierarchyPanel); // Opción para mostrar/ocultar Jerarquía
            ImGui::MenuItem("Proyecto", nullptr, &app->windowEditor->GetImGuiWindow()->showprojectPanel); // Opción para mostrar/ocultar Proyecto
            ImGui::EndMenu();

        }

        if (ImGui::BeginMenu("Ayuda")) {
            if (ImGui::BeginMenu("Estadisticas")) {
                ImGui::Text("FPS: %.1f", currentFPS);
                ImGui::PlotLines("FPS History", fpsHistory.data(), fpsHistory.size(), 0, nullptr, 0.0f, 100.0f, ImVec2(0, 80));

                // Obtener y mostrar solo la memoria utilizada por el proceso del motor
                size_t memUsedMB = GetMemoryUsage();
                ImGui::Text("Memory Usage (App): %zu MB", memUsedMB);

                ImGui::Separator();
                ImGui::Checkbox("Show FPS Overlay", &showOverlayFPS);  // Checkbox para mostrar overlay de FPS

                ImGui::Separator();
                ImGui::Text("Hardware & Software Info:");
                ImGui::Text("SDL Version: %d.%d.%d", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL);
                ImGui::Text("OpenGL Version: %s", glGetString(GL_VERSION));

                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Ventana overlay para mostrar FPS
    if (showOverlayFPS) {
        ImGui::SetNextWindowBgAlpha(0.5f);  // Transparencia del fondo
        
        ImGui::Begin("FPS Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
        ImGui::SetWindowPos(ImVec2(2, 20));  // Posición en la esquina superior izquierda
        ImGui::Text("Current FPS: %.1f", currentFPS);
        ImGui::End();
    }

    if (showDemo) {
        ImGui::ShowDemoWindow();
    }
}
