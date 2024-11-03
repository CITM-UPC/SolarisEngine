#include "WindowImGui.h"
#include "imgui.h"

#include <SDL2/SDL.h>
#include <iostream>

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

    // Configuraci�n de la fuente de iconos
    ImFontConfig config;
    config.MergeMode = true;   // Combinar con la fuente predeterminada, solo si es necesario
    config.PixelSnapH = true;  // Alinear p�xeles

    float iconFontSize = 16.0f;  // Ajusta el tama�o de icono

    // Usa la ruta absoluta
    std::string fontPath = "E:\\_GITHUB\\SolarisEngine\\SolarisEngine\\sdl2_simple_example\\sdl2_simple_example\\Library\\Fonts\\OpenFontIcons.ttf";

    // Rango de iconos (Unicode �rea privada)
    static const ImWchar icon_ranges[] = { ICON_MIN, ICON_MAX, 0 };

    // Verificar si el archivo existe antes de cargarlo
    if (!std::filesystem::exists(fontPath)) {
        std::cout << "Error: La fuente OpenFontIcons.ttf no se encuentra en la ruta especificada: " << fontPath << std::endl;
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
    hierarchyPanel = new PanelHierarchy(); // Crea el panel de jerarqu�a
    panels.push_back(hierarchyPanel); // A�ade el panel a la lista de panels

    inspectorPanel = new PanelInspector(); // Crea el panel del inspector
    panels.push_back(inspectorPanel); // A�ade el panel a la lista de panels

    consolaPanel = new PanelConsola(); // Crea el panel del inspector
    panels.push_back(consolaPanel); // A�ade el panel a la lista de panels

    projectPanel = new PanelProject(); // Crea el panel del inspector
    panels.push_back(projectPanel); // A�ade el panel a la lista de panels

    scenePanel = new PanelScene();
    panels.push_back(scenePanel);

    menuBar = new MenuBar();
}

void WindowImGui::Create() {
    // Aqu� puedes realizar inicializaciones adicionales si es necesario
    // Por ejemplo, puedes inicializar otros paneles si tienes m�s
}

void WindowImGui::Shutdown() {
    // Aqu� puedes realizar cualquier limpieza adicional si es necesario
}

void WindowImGui::HandleSDLEvent(SDL_Event& e) {
    // Aqu� puedes procesar eventos espec�ficos de SDL para ImGui
    // Ejemplo: ImGui_ImplSDL2_ProcessEvent(&e);
}

void WindowImGui::Render() {
    // Renderiza cada panel en la lista
    menuBar->Render();

    for (auto panel : panels) {
        panel->Render(); // Llama al m�todo de renderizado de cada panel
    }
}
