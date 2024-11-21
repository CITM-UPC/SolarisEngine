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
    : showDemo(false), fpsHistory{}, currentFPS(0.0f), frameCounter(0), showOverlayFPS(false) {

    ImGui::GetStyle().Colors[ImGuiCol_Text] = textColor;
    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = backgroundColor;
    ImGui::GetStyle().Colors[ImGuiCol_Border] = borderColor;
    ImGui::GetStyle().Colors[ImGuiCol_Button] = buttonColor;
    ImGui::GetStyle().Colors[ImGuiCol_Header] = headerColor;
    ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] = headerHoveredColor;
    ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] = headerActiveColor;
    ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = titleBgColor;
    ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = titleBgActiveColor;
    ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed] = titleBgCollapsedColor;
    ImGui::GetStyle().Colors[ImGuiCol_Tab] = tabColor;
    ImGui::GetStyle().Colors[ImGuiCol_TabActive] = tabSelectedColor;
    ImGui::GetStyle().Colors[ImGuiCol_TabHovered] = tabHoveredColor;
    ImGui::GetStyle().Colors[ImGuiCol_Tab] = tabUnselectedColor;
    ImGui::GetStyle().Colors[ImGuiCol_TabUnfocused] = tabUnfocusedColor;
    ImGui::GetStyle().Colors[ImGuiCol_TabUnfocusedActive] = tabUnfocusedActiveColor;


} // Añadido showOverlayFPS

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
          
            if (ImGui::MenuItem("ShowDemo")) {
                showDemo = !showDemo;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Preferencias")) {
                showPreferencePopup = true;
            }
           
           
            ImGui::EndMenu();
        }

       

        if (ImGui::BeginMenu("Assets")) {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("GameObject")) {
            if (ImGui::MenuItem("Duplicar")) {
                app->actualScene->AddGameObject(app->actualScene->GetSelectedGameObject()->Duplicate());
            }
            if (ImGui::MenuItem("Eliminar")) {
                app->actualScene->RemoveGameObject(app->actualScene->GetSelectedGameObject());
            }
            if (ImGui::BeginMenu("Primitivos")) {
                if (ImGui::MenuItem("Plane")) {
                    auto newGO = GameObject::Create("Plane");
                    auto mesh = newGO->AddComponent<Component_Mesh>();
                    mesh->GeneratePlaneMesh();
                    app->actualScene->AddGameObject(newGO);
                }
                if (ImGui::MenuItem("Cube")) {
                    auto newGO = GameObject::Create("Cube");
                    auto mesh = newGO->AddComponent<Component_Mesh>();
                    mesh->GenerateCubeMesh();
                    app->actualScene->AddGameObject(newGO);
                }
                if (ImGui::MenuItem("Triangle")) {
                    auto newGO = GameObject::Create("Triangle");
                    auto mesh = newGO->AddComponent<Component_Mesh>();
                    mesh->GenerateTriangleMesh();
                    app->actualScene->AddGameObject(newGO);
                }
                if (ImGui::MenuItem("Sphere")) {
                    auto newGO = GameObject::Create("Sphere");
                    auto mesh = newGO->AddComponent<Component_Mesh>();
                    mesh->GenerateSphereMesh();
                    app->actualScene->AddGameObject(newGO);
                }
                if (ImGui::MenuItem("Capsule")) {
                    auto newGO = GameObject::Create("Capsule");
                    auto mesh = newGO->AddComponent<Component_Mesh>();
                    mesh->GenerateCapsuleMesh();
                    app->actualScene->AddGameObject(newGO);
                }
                if (ImGui::MenuItem("Cylinder")) {
                    auto newGO = GameObject::Create("Cylinder");
                    auto mesh = newGO->AddComponent<Component_Mesh>();
                    mesh->GenerateCylinderMesh();
                    app->actualScene->AddGameObject(newGO);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Componentes")) {

            if (ImGui::MenuItem("Material")) {
                app->actualScene->GetSelectedGameObject()->AddComponent<Component_Material>();
            }
            if (ImGui::MenuItem("Mesh")) {
                app->actualScene->GetSelectedGameObject()->AddComponent<Component_Mesh>();
            }


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
            if (ImGui::MenuItem("About")) {
                SDL_OpenURL("https://github.com/Solaris-Group/SolarisEngine");
            }



            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();

       
    }
    if (showPreferencePopup) {
        //ImGui::OpenPopup("PreferencesPopup");
        PreferencePopup();
    }

    // Ventana overlay para mostrar FPS
    if (showOverlayFPS) {
        ImGui::SetNextWindowBgAlpha(0.5f);  // Transparencia del fondo
        
        ImGui::Begin("FPS Overlay", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
        ImGui::SetWindowPos(ImVec2(2, 20));  // Posición en la esquina superior izquierda
        ImGui::Text("FPS: %.1f", currentFPS);
        ImGui::End();
    }

    if (showDemo) {
        ImGui::ShowDemoWindow();

    }

   
}


void MenuBar::PreferencePopup() {


    ImGui::Begin("Configuraciones de Preferencias", &showPreferencePopup);

    // Separador para mejorar la legibilidad
    ImGui::Separator();

    // Categoría: Apariencia
    ImGui::Text("Apariencia");


    ImGui::ColorEdit4("Color del Texto", (float*)&textColor);
    ImGui::GetStyle().Colors[ImGuiCol_Text] = textColor;

    ImGui::ColorEdit4("Color de Fondo", (float*)&backgroundColor);
    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = backgroundColor;

    ImGui::ColorEdit4("Color de Bordes", (float*)&borderColor);
    ImGui::GetStyle().Colors[ImGuiCol_Border] = borderColor;

    ImGui::ColorEdit4("Color de Botones", (float*)&buttonColor);
    ImGui::GetStyle().Colors[ImGuiCol_Button] = buttonColor;


    // Color de la cabecera (normal)
    ImGui::ColorEdit4("Color de Cabecera", (float*)&headerColor);
    ImGui::GetStyle().Colors[ImGuiCol_Header] = headerColor;

    // Color de la cabecera al pasar el ratón
    ImGui::ColorEdit4("Color de Cabecera Hover", (float*)&headerHoveredColor);
    ImGui::GetStyle().Colors[ImGuiCol_HeaderHovered] = headerHoveredColor;

    // Color de la cabecera activa
    ImGui::ColorEdit4("Color de Cabecera Activa", (float*)&headerActiveColor);
    ImGui::GetStyle().Colors[ImGuiCol_HeaderActive] = headerActiveColor;


    // Color de la pestaña (normal)
    ImGui::ColorEdit4("Color de Pestaña", (float*)&tabColor);
    ImGui::GetStyle().Colors[ImGuiCol_Tab] = tabColor;

    // Color de la pestaña seleccionada
    ImGui::ColorEdit4("Color de Pestaña Seleccionada", (float*)&tabSelectedColor);
    ImGui::GetStyle().Colors[ImGuiCol_TabActive] = tabSelectedColor;

    // Color de la pestaña al pasar el ratón
    ImGui::ColorEdit4("Color de Pestaña Hover", (float*)&tabHoveredColor);
    ImGui::GetStyle().Colors[ImGuiCol_TabHovered] = tabHoveredColor;

    // Color de la pestaña no seleccionada (normal)
    ImGui::ColorEdit4("Color de Pestaña No Seleccionada", (float*)&tabUnselectedColor);
    ImGui::GetStyle().Colors[ImGuiCol_Tab] = tabUnselectedColor;

    // Color de la pestaña no seleccionada cuando la ventana no tiene foco
    ImGui::ColorEdit4("Color de Pestaña No Seleccionada Sin Foco", (float*)&tabUnfocusedColor);
    ImGui::GetStyle().Colors[ImGuiCol_TabUnfocused] = tabUnfocusedColor;

    // Color de la pestaña no seleccionada cuando la ventana no tiene foco pero está activa
    ImGui::ColorEdit4("Color de Pestaña No Seleccionada Activa", (float*)&tabUnfocusedActiveColor);
    ImGui::GetStyle().Colors[ImGuiCol_TabUnfocusedActive] = tabUnfocusedActiveColor;







    // Fondo de la cabecera de ventanas flotantes
    ImGui::ColorEdit4("Fondo de Cabecera", (float*)&titleBgColor);
    ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = titleBgColor;

    // Fondo de la cabecera activa de ventanas flotantes
    ImGui::ColorEdit4("Fondo de Cabecera Activa", (float*)&titleBgActiveColor);
    ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = titleBgActiveColor;

    // Fondo de la cabecera colapsada de ventanas flotantes
    ImGui::ColorEdit4("Fondo de Cabecera Colapsada", (float*)&titleBgCollapsedColor);
    ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed] = titleBgCollapsedColor;



    if (ImGui::SliderFloat("Transparencia", &ImGui::GetStyle().Alpha, 0.1f, 1.0f, "Alpha: %.2f")) {
        // El estilo Alpha se aplica automáticamente
    }

    if (ImGui::SliderFloat("Tamaño de Fuente", &fontSize, 10.0f, 30.0f, "Font Size: %.1f")) {
        ImGui::GetIO().FontGlobalScale = fontSize / 20.0f; // Escala proporcional
    }

    if (ImGui::SliderFloat("Redondeo de Bordes", &ImGui::GetStyle().WindowRounding, 0.0f, 12.0f, "%.1f")) {
        // Cambia el redondeo de bordes al vuelo
    }
    ImGuiStyle& style = ImGui::GetStyle();
    if (ImGui::SliderFloat("Espaciado entre Elementos", &style.ItemSpacing.x, 0.0f, 20.0f, "Horizontal: %.1f")) {}
    if (ImGui::SliderFloat("Espaciado Vertical", &style.ItemSpacing.y, 0.0f, 20.0f, "Vertical: %.1f")) {}




   

    ImGui::Separator(); // Otro separador

    // Categoría: Controles
    ImGui::Text("Controles");
    float currentSensitivity = app->cameraEditor->GetCameraSensivility();

    // Control deslizante para la sensibilidad del ratón
    if (ImGui::SliderFloat(u8"Sensibilidad del Ratón", &currentSensitivity, 0.001f, 1.5f, "Mouse Sensitivity: %.01f", 0.01f)) {
        app->cameraEditor->ChangeCameraSensivility(currentSensitivity);
    }


    float fov = app->cameraEditor->fov;
    // Control deslizante para la sensibilidad del ratón
    if (ImGui::SliderFloat(u8"FOV", &fov, 0.001f, 360.0f, "Fov: %.01f", 0.01f)) {
        app->cameraEditor->fov = fov;
    }






    ImGui::Separator(); // Otro separador


    if (ImGui::Button("Tema Oscuro")) {
        ImGui::StyleColorsDark();
    }
    if (ImGui::Button("Tema Claro")) {
        ImGui::StyleColorsLight();
    }
    if (ImGui::Button("Tema Clásico")) {
        ImGui::StyleColorsClassic();
    }


    ImGui::Separator(); // Otro separador


    if (ImGui::Button("Restaurar Valores Predeterminados")) {
        textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Resetear color de texto
        backgroundColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Resetear color de fondo
        //app->cameraEditor->ResetSettings(); // Resetear configuración de cámara
    }

    ImGui::Separator(); // Otro separador


    // Botones para aplicar cambios
    if (ImGui::Button("Cerrar")) {
        showPreferencePopup = false; // Cierra el panel
    }

    ImGui::End(); // Cierra la ventana de preferencias
}


