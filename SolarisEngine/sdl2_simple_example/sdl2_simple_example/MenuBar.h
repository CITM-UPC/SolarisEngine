#ifndef __MENU_BAR_H__
#define __MENU_BAR_H__

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <vector>

class MenuBar {
public:
    MenuBar();
    ~MenuBar() = default;

    void Render(); // M�todo para renderizar la barra de men�

private:
    bool showDemo;                  // Muestra la ventana de demostraci�n de ImGui
    std::vector<float> fpsHistory;  // Historial de FPS para graficar
    float currentFPS;               // FPS actual
    int frameCounter = 0;
    bool showOverlayFPS = false;
    // M�todo para obtener el consumo de memoria en diferentes plataformas
    size_t GetMemoryUsage();

};

#endif // __MENU_BAR_H__
