#ifndef __PANEL_H__
#define __PANEL_H__

#include "imgui.h"

class Panel {
public:
    virtual ~Panel() = default; // Destructor virtual
    virtual void Render() = 0; // Método puro virtual para renderizar el panel
    virtual void RenderContext() = 0; // Método puro virtual para renderizar el panel

    bool isHovered = false; // Inicializado a false por defecto

    // Actualizar el estado de hovered (debe llamarse en el renderizado)
    void UpdateHoveredStatus() {
        isHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup);
    }
};

#endif // __PANEL_H__
