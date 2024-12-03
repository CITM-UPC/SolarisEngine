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

    ReloadColors();


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
                    //auto newGO = GameObject::Create("Plane");
                    //auto mesh = newGO->AddComponent<Component_Mesh>();
                    ////mesh->GeneratePlaneMesh();
                    ////app->actualScene->AddGameObject(newGO);
                }
                if (ImGui::MenuItem("Cube")) {
                  /*  auto newGO = GameObject::Create("Cube");
                    auto mesh = newGO->AddComponent<Component_Mesh>();
                    mesh->GenerateCubeMesh();
                    app->actualScene->AddGameObject(newGO);*/
                }
                if (ImGui::MenuItem("Triangle")) {
                    //auto newGO = GameObject::Create("Triangle");
                    //auto mesh = newGO->AddComponent<Component_Mesh>();
                    //mesh->GenerateTriangleMesh();
                    //app->actualScene->AddGameObject(newGO);
                }
                if (ImGui::MenuItem("Sphere")) {
                  /*  auto newGO = GameObject::Create("Sphere");
                    auto mesh = newGO->AddComponent<Component_Mesh>();
                    mesh->GenerateSphereMesh();
                    app->actualScene->AddGameObject(newGO);*/
                }
                if (ImGui::MenuItem("Capsule")) {
                   /* auto newGO = GameObject::Create("Capsule");
                    auto mesh = newGO->AddComponent<Component_Mesh>();
                    mesh->GenerateCapsuleMesh();
                    app->actualScene->AddGameObject(newGO);*/
                }
                if (ImGui::MenuItem("Cylinder")) {
                   /* auto newGO = GameObject::Create("Cylinder");
                    auto mesh = newGO->AddComponent<Component_Mesh>();
                    mesh->GenerateCylinderMesh();
                    app->actualScene->AddGameObject(newGO);*/
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
            if (ImGui::MenuItem("Billboard")) {
                app->actualScene->GetSelectedGameObject()->AddComponent<Component_Billboard>();
            }
            if (ImGui::MenuItem("ParticleSystem")) {
                app->actualScene->GetSelectedGameObject()->AddComponent<Component_ParticleSystem>();
            }
            if (ImGui::MenuItem("Camera")) {
                app->actualScene->GetSelectedGameObject()->AddComponent<Component_Camera>();
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


    // Colores anteriores (previamente inicializados, puede ser valores predeterminados)
    static ImVec4 prevTextColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    static ImVec4 prevBackgroundColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevBorderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevButtonColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevButtonHoverColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevButtonActiveColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevHeaderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevHeaderHoveredColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevHeaderActiveColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevTabColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevTabSelectedColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevTabHoveredColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevTabUnselectedColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevTabUnfocusedColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevTabUnfocusedActiveColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevTitleBgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevTitleBgActiveColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevTitleBgCollapsedColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
    static ImVec4 prevFrameBgColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f); // Fondo normal
    static ImVec4 prevFrameBgHoveredColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f); // Fondo cuando el ratón pasa sobre el control
    static ImVec4 prevFrameBgActiveColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // Fondo cuando el control está activo


    // Usamos la función para actualizar los colores
    UpdateColorIfChanged("Color del Texto", textColor, prevTextColor, ImGuiCol_Text);
    UpdateColorIfChanged("Color de Fondo", backgroundColor, prevBackgroundColor, ImGuiCol_WindowBg);
    UpdateColorIfChanged("Color de Bordes", borderColor, prevBorderColor, ImGuiCol_Border);
    UpdateColorIfChanged("Color de Botones", buttonColor, prevButtonColor, ImGuiCol_Button);
    UpdateColorIfChanged("Color de Botones Hover", buttonHoveredColor, prevButtonHoverColor, ImGuiCol_ButtonHovered);
    UpdateColorIfChanged("Color de Botones Active", buttonActiveColor, prevButtonActiveColor, ImGuiCol_ButtonActive);


    UpdateColorIfChanged("Color de Cabecera", headerColor, prevHeaderColor, ImGuiCol_Header);
    UpdateColorIfChanged("Color de Cabecera Hover", headerHoveredColor, prevHeaderHoveredColor, ImGuiCol_HeaderHovered);
    UpdateColorIfChanged("Color de Cabecera Activa", headerActiveColor, prevHeaderActiveColor, ImGuiCol_HeaderActive);
    UpdateColorIfChanged("Color de Pestaña", tabColor, prevTabColor, ImGuiCol_Tab);
    UpdateColorIfChanged("Color de Pestaña Seleccionada", tabSelectedColor, prevTabSelectedColor, ImGuiCol_TabActive);
    UpdateColorIfChanged("Color de Pestaña Hover", tabHoveredColor, prevTabHoveredColor, ImGuiCol_TabHovered);
    UpdateColorIfChanged("Color de Pestaña No Seleccionada", tabUnselectedColor, prevTabUnselectedColor, ImGuiCol_Tab);
    UpdateColorIfChanged("Color de Pestaña No Seleccionada Sin Foco", tabUnfocusedColor, prevTabUnfocusedColor, ImGuiCol_TabUnfocused);
    UpdateColorIfChanged("Color de Pestaña No Seleccionada Activa", tabUnfocusedActiveColor, prevTabUnfocusedActiveColor, ImGuiCol_TabUnfocusedActive);
    UpdateColorIfChanged("Fondo de Cabecera", titleBgColor, prevTitleBgColor, ImGuiCol_TitleBg);
    UpdateColorIfChanged("Fondo de Cabecera Activa", titleBgActiveColor, prevTitleBgActiveColor, ImGuiCol_TitleBgActive);
    UpdateColorIfChanged("Fondo de Cabecera Colapsada", titleBgCollapsedColor, prevTitleBgCollapsedColor, ImGuiCol_TitleBgCollapsed);


    UpdateColorIfChanged("Color de Fondo de Sliders", frameBgColor, prevFrameBgColor, ImGuiCol_FrameBg);
    UpdateColorIfChanged("Color de Fondo de Sliders Hover", frameBgHoveredColor, prevFrameBgHoveredColor, ImGuiCol_FrameBgHovered);
    UpdateColorIfChanged("Color de Fondo de Sliders Activo", frameBgActiveColor, prevFrameBgActiveColor, ImGuiCol_FrameBgActive);




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


    // Crear el menú colapsable de Temas
    if (ImGui::CollapsingHeader("Seleccionar Tema", ImGuiTreeNodeFlags_DefaultOpen)) {

        // Botón para Tema Oscuro
        if (ImGui::Button("Tema Oscuro")) {
            ImGui::StyleColorsDark();
        }

        // Botón para Tema Claro
        if (ImGui::Button("Tema Claro")) {
            ImGui::StyleColorsLight();
        }

        // Botón para Tema Clásico
        if (ImGui::Button("Tema Clásico")) {
            ImGui::StyleColorsClassic();
        }

        // Botón para Tema Spooky
        if (ImGui::Button("Tema Spooky")) {
            SpookyTheme();  // Función personalizada para el tema Spooky
        }
        if (ImGui::Button("Tema Morado")) {
            PurpleTheme();  // Función personalizada para el tema Spooky
        }

       
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

void MenuBar::ReloadColors()
{
    ImGui::GetStyle().Colors[ImGuiCol_Text] = textColor;
    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = backgroundColor;
    ImGui::GetStyle().Colors[ImGuiCol_Border] = borderColor;
    ImGui::GetStyle().Colors[ImGuiCol_Button] = buttonColor;
    ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = buttonHoveredColor;
    ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = buttonActiveColor;
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

    ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = frameBgColor;
    ImGui::GetStyle().Colors[ImGuiCol_FrameBgHovered] = frameBgHoveredColor;
    ImGui::GetStyle().Colors[ImGuiCol_FrameBgActive] = frameBgActiveColor;
}

// Función para comparar dos ImVec4 y ver si son diferentes
bool MenuBar::AreColorsDifferent(const ImVec4& color1, const ImVec4& color2) {
    return (color1.x != color2.x || color1.y != color2.y || color1.z != color2.z || color1.w != color2.w);
}

// Función para actualizar el color solo si ha cambiado
void MenuBar::UpdateColorIfChanged(const char* label, ImVec4& color, ImVec4& prevColor, ImGuiCol idx) {
    if (ImGui::ColorEdit4(label, (float*)&color)) {
        if (AreColorsDifferent(color, prevColor)) {
            ImGui::GetStyle().Colors[idx] = color;
            prevColor = color;  // Guardamos el nuevo color
        }
    }
}
void MenuBar::SpookyTheme()
{
     textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Color del texto
     backgroundColor = ImVec4(0.0235, 0.0118, 0.0f, 1.0f); // Color de fondo
     borderColor = ImVec4(0.1686, 0.1020, 0.0000, 1.0f); // Color de los bordes
     buttonColor = ImVec4(0.6588, 0.3882, 0.0000, 1.0f); // Color de los botones

     buttonHoveredColor = ImVec4(0.831f, 0.670f, 0.439f, 1.000f); // Color de los botones cuando el ratón pasa sobre ellos
     buttonActiveColor = ImVec4(0.627f, 0.295f, 0.000f, 1.000f); // Color de los botones cuando están activos (pulsados)

     headerColor = ImVec4(0.593f, 0.213f, 0.006f, 1.000f);         // Azul para cabecera
     headerHoveredColor = ImVec4(0.716f, 0.210f, 0.000f, 1.000f); // Azul más claro al pasar el ratón
     headerActiveColor = ImVec4(0.931f, 0.657f, 0.000f, 1.000f);  // Azul más intenso cuando está activa

     tabColor = ImVec4(0.314f, 0.005f, 0.005f, 1.000f);           // Pestaña normal
     tabSelectedColor = ImVec4(0.912f, 0.402f, 0.000f, 1.000f);  // Pestaña seleccionada
     tabHoveredColor = ImVec4(0.696f, 0.553f, 0.000f, 1.000f);    // Pestaña con hover
     tabUnselectedColor = ImVec4(0.539f, 0.398f, 0.272f, 1.000f);      // Color de la pestaña no seleccionada
     tabUnfocusedColor = ImVec4(0.539f, 0.398f, 0.272f, 1.000f);    // Color de la pestaña no seleccionada sin foco
     tabUnfocusedActiveColor = ImVec4(0.868f, 0.332f, 0.000f, 1.000f);  // Color de la pestaña no seleccionada activa sin foco

     titleBgColor = ImVec4(0.176f, 0.088f, 0.000f, 1.000f);      // Fondo de cabecera inactiva
     titleBgActiveColor = ImVec4(0.152f, 0.027f, 0.000f, 1.000f); // Fondo de cabecera activa
     titleBgCollapsedColor = ImVec4(0.098f, 0.066f, 0.000f, 1.000f); // Fondo de cabecera colapsada

     frameBgColor = ImVec4(0.127f, 0.105f, 0.083f, 1.000f); // Fondo normal
     frameBgHoveredColor = ImVec4(0.495f, 0.278f, 0.002f, 1.000f); // Fondo cuando el ratón pasa sobre el control
     frameBgActiveColor = ImVec4(0.848f, 0.424f, 0.000f, 1.000f); // Fondo cuando el control está activo


     ReloadColors();

}

void MenuBar::PurpleTheme()
{
    // Colores morados
    textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Color del texto (blanco para contraste)
    backgroundColor = ImVec4(0.066f, 0.0f, 0.133f, 1.0f); // Color de fondo (morado oscuro)
    borderColor = ImVec4(0.188f, 0.062f, 0.188f, 1.0f); // Color de los bordes (morado oscuro)
    buttonColor = ImVec4(0.627f, 0.188f, 0.627f, 1.0f); // Color de los botones (morado claro)

    buttonHoveredColor = ImVec4(0.831f, 0.439f, 0.831f, 1.0f); // Color de los botones cuando el ratón pasa sobre ellos
    buttonActiveColor = ImVec4(0.627f, 0.0f, 0.627f, 1.0f); // Color de los botones cuando están activos (pulsados)

    // Colores de la cabecera
    headerColor = ImVec4(0.392f, 0.058f, 0.392f, 1.000f); // Morado para cabecera
    headerHoveredColor = ImVec4(0.576f, 0.188f, 0.576f, 1.000f); // Morado más claro al pasar el ratón
    headerActiveColor = ImVec4(0.831f, 0.439f, 0.831f, 1.000f);  // Morado más intenso cuando está activa

    // Colores de las pestañas
    tabColor = ImVec4(0.188f, 0.062f, 0.188f, 1.000f); // Pestaña normal (morado oscuro)
    tabSelectedColor = ImVec4(0.627f, 0.188f, 0.627f, 1.000f);  // Pestaña seleccionada (morado claro)
    tabHoveredColor = ImVec4(0.576f, 0.188f, 0.576f, 1.000f); // Pestaña con hover (morado brillante)
    tabUnselectedColor = ImVec4(0.392f, 0.058f, 0.392f, 1.000f); // Pestaña no seleccionada (morado más oscuro)
    tabUnfocusedColor = ImVec4(0.392f, 0.058f, 0.392f, 1.000f); // Pestaña no seleccionada sin foco
    tabUnfocusedActiveColor = ImVec4(0.831f, 0.439f, 0.831f, 1.000f);  // Pestaña no seleccionada activa sin foco

    // Colores de la cabecera
    titleBgColor = ImVec4(0.188f, 0.062f, 0.188f, 1.000f); // Fondo de cabecera inactiva (morado oscuro)
    titleBgActiveColor = ImVec4(0.576f, 0.188f, 0.576f, 1.000f); // Fondo de cabecera activa (morado brillante)
    titleBgCollapsedColor = ImVec4(0.392f, 0.058f, 0.392f, 1.000f); // Fondo de cabecera colapsada (morado suave)

    // Colores de los controles (sliders, inputs, etc.)
    frameBgColor = ImVec4(0.211f, 0.035f, 0.211f, 1.000f); // Fondo normal (morado oscuro)
    frameBgHoveredColor = ImVec4(0.576f, 0.188f, 0.576f, 1.000f); // Fondo cuando el ratón pasa sobre el control (morado brillante)
    frameBgActiveColor = ImVec4(0.831f, 0.439f, 0.831f, 1.000f); // Fondo cuando el control está activo (morado fuerte)

    ReloadColors(); // Recargar los colores definidos
}


