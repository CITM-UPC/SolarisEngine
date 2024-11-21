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
    ImVec4 backgroundColor = ImVec4(0.0235, 0.0118, 0.0f, 1.0f); // Color de fondo
    ImVec4 borderColor = ImVec4(0.1686, 0.1020, 0.0000, 1.0f); // Color de los bordes
    ImVec4 buttonColor = ImVec4(0.6588, 0.3882, 0.0000, 1.0f); // Color de los botones

    ImVec4 headerColor = ImVec4(0.593f, 0.213f, 0.006f, 1.000f);         // Azul para cabecera
    ImVec4 headerHoveredColor = ImVec4(0.716f, 0.210f, 0.000f, 1.000f); // Azul más claro al pasar el ratón
    ImVec4 headerActiveColor = ImVec4(0.931f, 0.657f, 0.000f, 1.000f);  // Azul más intenso cuando está activa

    ImVec4 tabColor = ImVec4(0.314f, 0.005f, 0.005f, 1.000f);           // Pestaña normal
    ImVec4 tabSelectedColor = ImVec4(0.912f, 0.402f, 0.000f, 1.000f);  // Pestaña seleccionada
    ImVec4 tabHoveredColor = ImVec4(0.696f, 0.553f, 0.000f, 1.000f);    // Pestaña con hover
    ImVec4 tabUnselectedColor = ImVec4(0.539f, 0.398f, 0.272f, 1.000f);      // Color de la pestaña no seleccionada
    ImVec4 tabUnfocusedColor = ImVec4(0.539f, 0.398f, 0.272f, 1.000f);    // Color de la pestaña no seleccionada sin foco
    ImVec4 tabUnfocusedActiveColor = ImVec4(0.868f, 0.332f, 0.000f, 1.000f);  // Color de la pestaña no seleccionada activa sin foco



    ImVec4 titleBgColor = ImVec4(0.176f, 0.088f, 0.000f, 1.000f);      // Fondo de cabecera inactiva
    ImVec4 titleBgActiveColor = ImVec4(0.152f, 0.027f, 0.000f, 1.000f); // Fondo de cabecera activa
    ImVec4 titleBgCollapsedColor = ImVec4(0.098f, 0.066f, 0.000f, 1.000f); // Fondo de cabecera colapsada

    ImVec4 frameBgColor = ImVec4(0.127f, 0.105f, 0.083f, 1.000f); // Fondo normal
    ImVec4 frameBgHoveredColor = ImVec4(0.495f, 0.278f, 0.002f, 1.000f); // Fondo cuando el ratón pasa sobre el control
    ImVec4 frameBgActiveColor = ImVec4(0.848f, 0.424f, 0.000f, 1.000f); // Fondo cuando el control está activo




    bool showPreferencePopup = false;


    void PreferencePopup();

    void ReloadColors();

    bool AreColorsDifferent(const ImVec4& color1, const ImVec4& color2);


    void UpdateColorIfChanged(const char* label, ImVec4& color, ImVec4& prevColor, ImGuiCol idx);

    void SpookyTheme();

};

#endif // __MENU_BAR_H__
