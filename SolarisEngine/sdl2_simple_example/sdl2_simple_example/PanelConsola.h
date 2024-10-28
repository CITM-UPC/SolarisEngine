#pragma once
#ifndef __PANEL_CONSOLA_H__
#define __PANEL_CONSOLA_H__

#include "Panel.h" // Incluir la clase base Panel
#include "Component.h"
#include "Component_Material.h"
#include "Component_Mesh.h"
#include "Component_Transform.h"

#include "imgui.h"
class PanelConsola : public Panel { // Hereda de Panel
public:
    PanelConsola();
    ~PanelConsola();

    void Render() override; // Implementa el método Render
    void AddLog(const std::string& message);



private:
    ImGuiTextBuffer logBuffer;   // Buffer for storing log messages
    ImGuiTextFilter filter;
};

#endif // __PANEL_INSPECTOR_H__
