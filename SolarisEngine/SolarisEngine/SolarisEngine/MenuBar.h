#ifndef __MENU_BAR_H__
#define __MENU_BAR_H__

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <vector>

class MenuBar {
public:
    MenuBar();
    ~MenuBar() = default;

    void Render(); // Método para renderizar la barra de menú

private:
    bool showDemo;                  // Muestra la ventana de demostración de ImGui
    std::vector<float> fpsHistory;  // Historial de FPS para graficar
    float currentFPS;               // FPS actual
    int frameCounter = 0;
    bool showOverlayFPS = false;
    // Método para obtener el consumo de memoria en diferentes plataformas
    size_t GetMemoryUsage();

    float fontSize = 14.0f; // Tamaño de la tipografía
    ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // Color del texto
    ImVec4 backgroundColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f); // Color de fondo

    bool showPreferencePopup = false;


    void PreferencePopup();


};

#endif // __MENU_BAR_H__
