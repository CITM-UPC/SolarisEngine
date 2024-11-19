#include "WindowImGui.h"
#include "imgui.h"

#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>
#include "App.h"

WindowImGui::WindowImGui()
    : hierarchyPanel(nullptr), inspectorPanel(nullptr) {
    LoadFonts();
    CreatePanels(); // Crea los paneles al inicializar
}

WindowImGui::~WindowImGui() {
    // Limpieza de los paneles
    for (auto panel : panels) {
        delete panel;
    }
    panels.clear();
}



void WindowImGui::LoadFonts() {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();  // Cargar la fuente predeterminada de ImGui

    // Configuración de la fuente de iconos
    ImFontConfig config;
    config.MergeMode = true;   // Combinar con la fuente predeterminada, solo si es necesario
    config.PixelSnapH = true;  // Alinear píxeles
    config.GlyphOffset = ImVec2(3.0f, 3.0f);

    float iconFontSize = 13.0f;  // Ajusta el tamaño de icono

    // Usa la ruta absoluta
    std::string fontPath = std::string(FONTS_DIR) + "/OpenFontIcons.ttf";

    // Rango de iconos (Unicode área privada)
    static const ImWchar icon_ranges[] = { ICON_MIN, ICON_MAX, 0 };

    // Verificar si el archivo existe antes de cargarlo
    if (!std::filesystem::exists(fontPath)) {
        std::cout << "Error: La fuente OpenFontIcons.ttf no se encuentra en la ruta especificada: " << fontPath << std::endl;
        std::stringstream ss;
        ss << "Error: La fuente OpenFontIcons.ttf no se encuentra en la ruta especificada: " << fontPath << std::endl;
        app->windowEditor->GetImGuiWindow()->consolaPanel->AddLog(ss.str());
    }
    else {
        iconFont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), iconFontSize, &config, icon_ranges);
        if (iconFont) {
            printf("OpenFontIcons.ttf cargada correctamente desde %s\n", fontPath.c_str());
        }
        else {
            printf("Error al cargar OpenFontIcons.ttf desde %s\n", fontPath.c_str());
        }
    }



    io.Fonts->Build();  // Reconstruye las fuentes para ImGui
}


void WindowImGui::CreatePanels() {
    hierarchyPanel = new PanelHierarchy(); // Crea el panel de jerarquía
    panels.push_back(hierarchyPanel); // Añade el panel a la lista de panels

    inspectorPanel = new PanelInspector(); // Crea el panel del inspector
    panels.push_back(inspectorPanel); // Añade el panel a la lista de panels

    consolaPanel = new PanelConsola(); // Crea el panel del inspector
    panels.push_back(consolaPanel); // Añade el panel a la lista de panels

    projectPanel = new PanelProject(); // Crea el panel del inspector
    panels.push_back(projectPanel); // Añade el panel a la lista de panels

    scenePanel = new PanelScene();
    panels.push_back(scenePanel);

    menuBar = new MenuBar();

    
}

void WindowImGui::Create() {
    // Aquí puedes realizar inicializaciones adicionales si es necesario
    // Por ejemplo, puedes inicializar otros paneles si tienes más
}

void WindowImGui::Shutdown() {
    // Aquí puedes realizar cualquier limpieza adicional si es necesario
}

void WindowImGui::HandleSDLEvent(SDL_Event& e) {
    // Aquí puedes procesar eventos específicos de SDL para ImGui
    // Ejemplo: ImGui_ImplSDL2_ProcessEvent(&e);
}

void WindowImGui::Render() {
    // Renderiza cada panel en la lista
    menuBar->Render();

    for (auto panel : panels) {
        panel->Render(); // Llama al método de renderizado de cada panel
    }
}
