#pragma once

#include <imgui.h>
#include <imgui_impl_sdl2.h>

class MenuBar {
public:
    MenuBar() = default;
    ~MenuBar() = default;

    void Render(); // Método para renderizar la barra de menú
};
